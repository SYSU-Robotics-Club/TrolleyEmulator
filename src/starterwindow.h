#ifndef STARTERWINDOW_H
#define STARTERWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <list>
#include <QPointF>
#include <QTimer>
#include <QPaintEvent>
#include <QMouseEvent>

#include "car.h"
#include "buildcarwindow.h"

namespace Ui {
class StarterWindow;
}

class StarterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StarterWindow(QWidget *parent = 0);
    ~StarterWindow();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

private slots:
    void on_quitPushButton_clicked();

    void on_startPushButton_clicked();

    void on_pausePushButton_clicked();

    void worldAct();

    void on_savePushButton_clicked();

    void on_importPushButton_clicked();

private:
    Ui::StarterWindow *ui;
    QTimer timer;
    BuildCarWindow *buildCarWindow;

    bool worldStop;
};

#endif // STARTERWINDOW_H
