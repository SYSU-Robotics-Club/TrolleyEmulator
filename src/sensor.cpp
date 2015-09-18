#include "sensor.h"
#include "coreservice.h"

Sensor::Sensor(qreal positionX, qreal positionY, qreal initialRadius) :
    position(positionX, positionY), radius(initialRadius)
{

}

bool Sensor::sensorDetect(const QTransform &transform)
{
    QImage *targetMap = CoreService::getInstance()->getMap();
    QRgb black = LINE_COLOR.rgb();
    int startRow = (int) (transform.map(position).x() - radius);
    int startColumn = (int) (transform.map(position).y() - radius);
    int searchLength = (int) (2 * radius);
    int endRow = startRow + searchLength;
    int endColumn = startColumn + searchLength;

    // avoid out of bound
    startColumn = (startColumn < 0)?0:startColumn;
    startRow = (startRow < 0)?0:startRow;
    endRow %= targetMap->width();
    endColumn %= targetMap->height();


    for (int rowCount = startRow; rowCount <= endRow; rowCount++) {
        for (int columnCount = startColumn; columnCount <= endColumn; columnCount++) {
            if (targetMap->pixel(rowCount, columnCount) == black) {
                return true;
            }
        }
    }

    return false;
}

void Sensor::drawSelf(QPainter *painter)
{
    painter->drawEllipse(position, radius, radius);
}
QPointF Sensor::getPosition() const
{
    return position;
}

void Sensor::setPosition(const QPointF &value)
{
    position = value;
}
qreal Sensor::getRadius() const
{
    return radius;
}

void Sensor::setRadius(const qreal &value)
{
    radius = value;
}
