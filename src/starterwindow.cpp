#include "starterwindow.h"
#include "ui_starterwindow.h"

#include <math.h>
#include <time.h>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>

#include "coreservice.h"
#define TOOLBAR_HEIGHT 100
#define BUILD_CAR_WINDOW_WIDTH 200

StarterWindow::StarterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StarterWindow),
    worldStop(true)
{
    ui->setupUi(this);
    buildCarWindow = new BuildCarWindow(ui->buildCarFrame);
    buildCarWindow->move(0, 0);
    buildCarWindow->show();

    this->setFixedSize(MAX_WIDTH + BUILD_CAR_WINDOW_WIDTH, MAX_HEIGHT + TOOLBAR_HEIGHT);
    this->move((QApplication::desktop()->width() - this->width()) / 2,
               (QApplication::desktop()->height() - this->height()) / 2);
    this->setWindowFlags(Qt::FramelessWindowHint);

    srand(time(NULL));

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(worldAct()));
    timer.start(20);
}

StarterWindow::~StarterWindow()
{
    delete buildCarWindow;
    delete ui;
}

void StarterWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPointF(0, 0), *(CoreService::getInstance()->getMap()));

    painter.save();
    QPen pen = painter.pen();
    pen.setWidth(5);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoint(CoreService::getInstance()->getBuildMapCurrentPoint());
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    painter.drawPoint(CoreService::getInstance()->getBuildMapTargetPoint());
    painter.restore();

    CoreService::getInstance()->drawCar(&painter);

    event->accept();
}


void StarterWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        CoreService::getInstance()->setTargetPoint(event->pos());
    } else if (event->button() == Qt::RightButton) {
        CoreService::getInstance()->setCurrentPoint(event->pos());
    }
}

void StarterWindow::on_quitPushButton_clicked()
{
    QApplication::quit();
}

void StarterWindow::on_startPushButton_clicked()
{
    worldStop = false;
}

void StarterWindow::on_pausePushButton_clicked()
{
    worldStop = true;
}

void StarterWindow::worldAct()
{
    if (worldStop) {
        return;
    }
    CoreService::getInstance()->buildMapStep();
    CoreService::getInstance()->itemAction();
}

void StarterWindow::on_savePushButton_clicked()
{
    const QString filename = QFileDialog::getSaveFileName();
    if (filename.length()) {
        CoreService::getInstance()->saveStateToFile(filename);
    }
}

void StarterWindow::on_importPushButton_clicked()
{
    const QString filename = QFileDialog::getOpenFileName();
    if (filename.length()) {
        CoreService::getInstance()->recoverStateFromFile(filename);
    }
}
