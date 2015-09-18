#ifndef SENSOR_H
#define SENSOR_H

#include <QPointF>
#include <QTransform>
#include <QPainter>

class Sensor
{
public:
    Sensor(qreal positionX, qreal positionY, qreal initialRadius);
    virtual bool sensorDetect(const QTransform &transform);
    virtual void drawSelf(QPainter *painter);

    QPointF getPosition() const;
    void setPosition(const QPointF &value);

    qreal getRadius() const;
    void setRadius(const qreal &value);

protected:
    QPointF position;
    qreal radius;
};

#endif // SENSOR_H
