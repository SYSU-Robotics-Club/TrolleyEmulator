#include "ekurisplashscreen.h"

#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <math.h>

#define EKURISPLASHSCREEN_WIDTH 400
#define EKURISPLASHSCREEN_HEIGHT 200
#define EKURI_REFLESH_RATE 16
#define ERROR_STAGE -1

EkuriSplashScreen::EkuriSplashScreen() : backgroundPixmap(EKURISPLASHSCREEN_WIDTH, EKURISPLASHSCREEN_HEIGHT)
{
    init();
}

EkuriSplashScreen::~EkuriSplashScreen()
{
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

void EkuriSplashScreen::init()
{
    stage = 0;
    frameCount = 0;
    currentProgress = 0;
    rotateLineCurrentAngle = 0;
    mainFontSize = 48;
    textBlinkSpeed = 75;
    oneStageTotalFrame = 1500 / EKURI_REFLESH_RATE;
    lineLength = EKURISPLASHSCREEN_WIDTH / 8;
    lineLengthChangeFlag = 0.5;
    rotateMidPointF.rx()= EKURISPLASHSCREEN_WIDTH / 2;
    rotateMidPointF.ry()= EKURISPLASHSCREEN_HEIGHT / 2;
    rotateFinalPointF.rx() = EKURISPLASHSCREEN_WIDTH - mainFontSize / 2;
    rotateFinalPointF.ry() = EKURISPLASHSCREEN_HEIGHT / 2 - mainFontSize / 2;
    rotateFinalLength = mainFontSize / 4;
    rotateErrorPointF.rx() = mainFontSize;
    rotateErrorPointF.ry() = EKURISPLASHSCREEN_HEIGHT / 2 - mainFontSize / 2;

    progressLineStartPosition.rx() = 0;
    progressLineStartPosition.ry() = (EKURISPLASHSCREEN_HEIGHT / 2 + mainFontSize);
    progressLineLength = EKURISPLASHSCREEN_WIDTH;

    this->move((QApplication::desktop()->width() - this->width()) / 2, (QApplication::desktop()->height() - this->height()) / 2);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

    backgroundPixmap.fill(QColor(0, 0, 0, 0));
    setPixmap(backgroundPixmap);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(EKURI_REFLESH_RATE);
}

void EkuriSplashScreen::mousePressEvent(QMouseEvent *event)
{
    if (stage == ERROR_STAGE && event->button() == Qt::LeftButton) {
        QApplication::quit();
    }
    event->accept();
}

void EkuriSplashScreen::drawContents(QPainter *painter)
{
    frameCount++;
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    stageSpecifyAction(painter);

    painter->restore();
}

void EkuriSplashScreen::drawRotateLine(QPainter *painter, QColor lineColor)
{
    painter->save();

    rotateLineCurrentAngle += (2 * M_PI / oneStageTotalFrame);

    QPointF firstPointF(cos(rotateLineCurrentAngle) * lineLength, sin(rotateLineCurrentAngle) * lineLength);
    QPointF secondPointF = -firstPointF;
    firstPointF += rotateMidPointF;
    secondPointF += rotateMidPointF;

    QPen pen;
    pen.setWidth(3);
    pen.setColor(lineColor);
    painter->setPen(pen);
    painter->drawLine(firstPointF, secondPointF);

    painter->restore();
}

void EkuriSplashScreen::drawProgressLine(QPainter *painter, QColor progressLineColor)
{
    painter->save();

    QPointF progressCurrentPosition = progressLineStartPosition;
    progressCurrentPosition.rx() += progressLineLength * currentProgress;

    QPen pen = painter->pen();
    pen.setWidth(2);
    pen.setColor(progressLineColor);
    painter->setPen(pen);
    painter->drawLine(progressLineStartPosition, progressCurrentPosition);

    painter->restore();
}

void EkuriSplashScreen::drawMainSplashLayout(QPainter *painter, bool blink,
                                             QColor mainTextColor, QColor statusTextColor)
{

    painter->save();

    QPixmap mainSplashLayoutBackground(EKURISPLASHSCREEN_WIDTH, mainFontSize * 3);
    mainSplashLayoutBackground.fill(QColor("#34495e"));
    painter->drawPixmap(0, (EKURISPLASHSCREEN_HEIGHT / 2 - mainFontSize * 3 / 2), mainSplashLayoutBackground);


    QPen pen = painter->pen();

    if (blink) {
        QColor penColor = pen.color();
        penColor.setAlphaF(abs(frameCount % (textBlinkSpeed * 2) - textBlinkSpeed) * 1.0 / textBlinkSpeed);
        pen.setColor(penColor);
    }

    pen.setWidth(3);

    QFont font;
    font.setPixelSize(mainFontSize);
    font.setBold(true);
    font.setItalic(true);
    painter->setFont(font);

    pen.setColor(mainTextColor);
    painter->setPen(pen);
    painter->drawText(0, 0, EKURISPLASHSCREEN_WIDTH, EKURISPLASHSCREEN_HEIGHT,
                      Qt::AlignCenter, tr("ekuri software"));

    font.setPixelSize(mainFontSize / 4);
    font.setItalic(false);
    painter->setFont(font);
    pen.setColor(statusTextColor);
    painter->setPen(pen);
    painter->drawText(EKURISPLASHSCREEN_WIDTH - mainFontSize / 4 * statusInformation.length(), EKURISPLASHSCREEN_HEIGHT / 2 + mainFontSize / 2,
                      mainFontSize / 4 * statusInformation.length(), mainFontSize / 4, Qt::AlignCenter, statusInformation);

    painter->restore();
}

void EkuriSplashScreen::drawErrorLayout(QPainter *painter)
{
    painter->save();

    QPen pen = painter->pen();
    pen.setColor(Qt::white);
    pen.setWidth(4);

    QFont font = painter->font();
    font.setBold(true);
    font.setPixelSize(mainFontSize / 2);

    painter->setPen(pen);
    painter->setFont(font);

    QPixmap errorBackground(EKURISPLASHSCREEN_WIDTH, mainFontSize * 2);
    errorBackground.fill(QColor("#c0392b"));
    painter->drawPixmap(0, (EKURISPLASHSCREEN_HEIGHT / 2 - mainFontSize), errorBackground);

    painter->drawText(0, (EKURISPLASHSCREEN_HEIGHT - mainFontSize) / 2,
                      EKURISPLASHSCREEN_WIDTH, mainFontSize, Qt::AlignCenter,
                      errorInformation);

    painter->restore();
}

void EkuriSplashScreen::stageSpecifyAction(QPainter *painter)
{
    switch (stage) {
    case 0:
        if (frameCount > oneStageTotalFrame) {
            stageChangeTo(1);
        }
        drawRotateLine(painter);

        break;
    case 1:
        lineLength -= lineLengthChangeFlag;

        if (lineLength > mainFontSize) {
            stageChangeTo(2);
        }
        if (lineLength <= 0) {
            lineLengthChangeFlag = -lineLengthChangeFlag;
            oneStageTotalFrame = 500 / EKURI_REFLESH_RATE;
        }
        if (lineLengthChangeFlag < 0) {
            drawRotateLine(painter, QColor("#16a085"));
        } else {
            drawRotateLine(painter);
        }

        break;
    case 2:

        if (rotateMidPointF.toPoint() != progressLineStartPosition.toPoint()) {
            rotateMidPointF += (1.0 / oneStageTotalFrame) * (progressLineStartPosition - rotateMidPointF);
        } else {
            stageChangeTo(3);
        }

        if (lineLength > 0) {
                lineLength--;
        } else {
            stageChangeTo(3);
        }
        drawRotateLine(painter, QColor("#27ae60"));

        break;
    case 3:
        rotateMidPointF.rx() += progressLineLength / (oneStageTotalFrame * 3);
        currentProgress += 1.0 / (oneStageTotalFrame * 3);
        if (currentProgress > 0.5) {
            stageChangeTo(4);
        }

        drawMainSplashLayout(painter, false, Qt::white, QColor("#3498db"));
        drawRotateLine(painter, QColor("#bdc3c7"));
        drawProgressLine(painter, QColor("#bdc3c7"));

        break;
    case 4:
        if (rotateMidPointF != rotateFinalPointF) {
            rotateMidPointF += 1.0 / oneStageTotalFrame * (rotateFinalPointF - rotateMidPointF);
        }
        currentProgress += 1.0 / (oneStageTotalFrame * 3);
        if (currentProgress > 1) {
            stageChangeTo(5);
        }

        drawMainSplashLayout(painter, false, Qt::white, QColor("#3498db"));
        drawRotateLine(painter, QColor("#bdc3c7"));
        drawProgressLine(painter, QColor("#bdc3c7"));

        break;
    case 5:
        if (rotateMidPointF.toPoint() != rotateFinalPointF.toPoint()) {
            rotateMidPointF += 1.0 / oneStageTotalFrame * (rotateFinalPointF - rotateMidPointF);
        } else {
            //stageChangeTo(ERROR_STAGE);

        }
        if (lineLength != rotateFinalLength) {
            lineLength += 1.0 / oneStageTotalFrame * (rotateFinalLength - lineLength);
        }

        drawMainSplashLayout(painter, false, Qt::white, QColor("#3498db"));
        drawRotateLine(painter, QColor("#bdc3c7"));
        drawProgressLine(painter, QColor("#bdc3c7"));

        break;

    case ERROR_STAGE:
        drawErrorLayout(painter);

        if (rotateMidPointF.toPoint() != rotateErrorPointF.toPoint()) {
            rotateMidPointF += 1.0 / oneStageTotalFrame * (rotateErrorPointF - rotateMidPointF);
        } else if (oneStageTotalFrame >= 200 / EKURI_REFLESH_RATE) {
            if (frameCount % 5 == 0) {
                oneStageTotalFrame--;
            }
        }
        if (lineLength > 1) {
            if (frameCount % 50 == 0) {
                lineLength--;
            }
        }
        if (lineLength > 1) {
            drawRotateLine(painter, Qt::white);
        }

        break;

    default:
        break;
    }
}

void EkuriSplashScreen::stageChangeTo(int nextStage)
{
    stage = nextStage;
    switch (stage) {
    case 1:
        lineLength = mainFontSize;
        break;

    case 2:
        // guarantee positive value
        lineLengthChangeFlag *= lineLengthChangeFlag;
        break;

    case 3:
        lineLength = mainFontSize / 16;
        statusInformation = tr("Loading Data...");
        oneStageTotalFrame = 1000 / EKURI_REFLESH_RATE;
        rotateMidPointF = progressLineStartPosition;

        break;

    case 4:
        statusInformation = tr("Seting up Window...");
        oneStageTotalFrame = 500 / EKURI_REFLESH_RATE;
        break;

    case 5:
        statusInformation = tr("Completing...");

        break;

    case ERROR_STAGE:
        errorInformation = "No Ekuri Application Found!";
        oneStageTotalFrame = 500 / EKURI_REFLESH_RATE;
        break;

    default:
        break;
    }
}
void EkuriSplashScreen::setErrorInformation(const QString &value)
{
    errorInformation = value;
}

void EkuriSplashScreen::setStatusInformation(const QString &value)
{
    statusInformation = value;
}

void EkuriSplashScreen::setCurrentProgress(const qreal &value)
{
    currentProgress = value;
}

