#ifndef EKURISPLASHSCREEN_H
#define EKURISPLASHSCREEN_H

#include <QSplashScreen>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QString>

class EkuriSplashScreen : public QSplashScreen
{
public:
    explicit EkuriSplashScreen();
    ~EkuriSplashScreen();
    void init();

    void setCurrentProgress(const qreal &value);

    void setStatusInformation(const QString &value);

    void setErrorInformation(const QString &value);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void drawContents(QPainter *painter);
    virtual void drawRotateLine(QPainter *painter, QColor lineColor = Qt::black);
    virtual void drawProgressLine(QPainter *painter, QColor progressLineColor = Qt::black);
    virtual void drawMainSplashLayout(QPainter *painter, bool blink, QColor mainTextColor = Qt::black,
                                      QColor statusTextColor = Qt::black);
    virtual void drawErrorLayout(QPainter *painter);
    virtual void stageSpecifyAction(QPainter *painter);
    virtual void stageChangeTo(int nextStage);

    QTimer timer;
    QPixmap backgroundPixmap;

    qreal rotateLineCurrentAngle;
    QPointF rotateMidPointF;
    QPointF rotateFinalPointF;
    QPointF rotateErrorPointF;
    qreal rotateFinalLength;

    QString statusInformation;
    QString errorInformation;
    QPointF progressLineStartPosition;
    qreal progressLineLength;
    qreal currentProgress;

    int frameCount;
    int textBlinkSpeed;
    int stage;

    int mainFontSize;

    int oneStageTotalFrame;

    qreal lineLength;
    qreal lineLengthChangeFlag;

};

#endif // EKURISPLASHSCREEN_H
