#ifndef BUILDCARWINDOW_H
#define BUILDCARWINDOW_H

#include <QMouseEvent>
#include <QWidget>
#include <QPaintEvent>

namespace Ui {
class BuildCarWindow;
}

class BuildCarWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BuildCarWindow(QWidget *parent = 0);
    ~BuildCarWindow();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::BuildCarWindow *ui;
};

#endif // BUILDCARWINDOW_H
