#ifndef CORESERVICE_H
#define CORESERVICE_H

#include <QImage>
#include <QColor>

#include "car.h"

#define MAX_WIDTH 800
#define MAX_HEIGHT 500
#define LINE_COLOR (QColor("#bdc3c7"))

class CoreService
{
public:
    ~CoreService() {
        delete map;
        delete car;
    }

    inline static CoreService *getInstance() {
        return instance;
    }

    inline QImage *getMap() const {
        return map;
    }

    inline void drawCar(QPainter *painter) {
        car->drawComponents(painter);
    }

    inline void itemAction() {
        car->act();
    }

    Car *getCar() const;
    virtual void buildMap();
    virtual void setTargetPoint(const QPointF &target);
    virtual void setCurrentPoint(const QPointF &target);
    virtual bool getNextPoint();
    virtual void buildMapStep();
    virtual bool saveStateToFile(const QString &filename);
    virtual bool recoverStateFromFile(const QString &filename);

    QPointF getBuildMapTargetPoint() const;

    QPointF getBuildMapCurrentPoint() const;

private:
    CoreService() : buildMapCurrentPointMovable(false), buildMapCurrentPointVelocity(0, 0) {
        map = new QImage(MAX_WIDTH, MAX_HEIGHT, QImage::Format_ARGB32);
        map->fill(QColor(0, 0, 0, 0));
        car = new Car();

        //resetPositionRandomly(currentPosition);
        buildMapCurrentPoint.rx() = 200;
        buildMapCurrentPoint.ry() = 100;
    }

    static CoreService *instance;
    QImage *map;
    Car *car;

    std::list<QPointF> pointsList;

    bool buildMapCurrentPointMovable;
    QPointF buildMapCurrentPointVelocity;
    QPointF buildMapTargetPoint;
    QPointF buildMapCurrentPoint;

    qreal distanceBetween(const QPointF &left, const QPointF &right);
    void resetPositionRandomly(QPointF &target);
};

#endif // CORESERVICE_H
