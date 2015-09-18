#include "buildcarwindow.h"
#include "ui_buildcarwindow.h"
#include "coreservice.h"

BuildCarWindow::BuildCarWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuildCarWindow)
{
    ui->setupUi(this);
}

BuildCarWindow::~BuildCarWindow()
{
    delete ui;
}

void BuildCarWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPointF newSensorPosition = ((QPointF)(event->pos() - ui->carLabel->geometry().center())) / 4;
        CoreService::getInstance()->getCar()->addSensor(Sensor(newSensorPosition.x(),
                                                               newSensorPosition.y(),
                                                               5));
        update();
    } else if (event->button() == Qt::RightButton) {
        CoreService::getInstance()->getCar()->removeLastSensor();
        update();
    }
}

void BuildCarWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    foreach (Sensor s, CoreService::getInstance()->getCar()->getSensorsList()) {
        painter.drawEllipse(s.getPosition() * 4 + ui->carLabel->geometry().center(),
                            s.getRadius() * 2, s.getRadius() * 2);
    }

    event->accept();
}

void BuildCarWindow::closeEvent(QCloseEvent *event)
{
    QApplication::quit();
    event->accept();
}
