#ifndef CAR_H
#define CAR_H

#include <QPainter>
#include <list>

#include "sensor.h"

class Car
{
public:
    Car();
    virtual ~Car();
    virtual void carMoveFoward();
    virtual void drawComponents(QPainter *painter);
    virtual void turnRight();
    virtual void turnLeft();
    virtual void act();

    void addSensor(const Sensor &newSensor);
    void removeLastSensor();

    std::list<Sensor> getSensorsList() const;

protected:
    QPointF carPosition;
    int carDirection;
    qreal carSpeed;

    std::list<Sensor> sensorsList;
};

#endif // CAR_H
