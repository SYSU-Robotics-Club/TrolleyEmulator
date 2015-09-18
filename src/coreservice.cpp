#include "coreservice.h"

#include <math.h>
#include <QXmlStreamWriter>

CoreService* CoreService::instance = new CoreService();

Car *CoreService::getCar() const
{
    return car;
}

void CoreService::buildMap()
{
    QPainter painter(map);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen = painter.pen();
    pen.setWidth(3);
    pen.setColor(LINE_COLOR);
    painter.setPen(pen);

    std::list<QPointF>::iterator iter = pointsList.begin();
    while (iter != pointsList.end()) {
        painter.drawPoint(*iter);
        iter = pointsList.erase(iter);
    }
}

void CoreService::setTargetPoint(const QPointF &target)
{
    buildMapTargetPoint = target;
    buildMapCurrentPointMovable = true;
}

void CoreService::setCurrentPoint(const QPointF &target)
{
    buildMapCurrentPoint = target;
}

bool CoreService::getNextPoint()
{
    if (buildMapCurrentPointMovable) {
        if ((abs(buildMapCurrentPoint.x() - buildMapTargetPoint.x()) < 1) && (abs(buildMapCurrentPoint.y() - buildMapTargetPoint.y())< 1)) {
            return false;
        } else {
            buildMapCurrentPointVelocity += (buildMapTargetPoint - buildMapCurrentPoint) / (distanceBetween(QPointF(0, 0), buildMapTargetPoint - buildMapCurrentPoint) * 10);
            qreal velocityLength = distanceBetween(QPointF(0, 0), buildMapCurrentPointVelocity);
            if (velocityLength == 0) {
                return false;
            }
            buildMapCurrentPointVelocity /= velocityLength / 2;

            buildMapCurrentPoint += buildMapCurrentPointVelocity;
            pointsList.push_back(buildMapCurrentPoint);
            return true;
        }
    }
    return false;
}

void CoreService::buildMapStep()
{
    buildMapCurrentPointMovable = getNextPoint();
    if (buildMapCurrentPointMovable) {
        buildMap();
    }
}

bool CoreService::saveStateToFile(const QString &filename)
{
    map->save(filename);
    return true;
}

bool CoreService::recoverStateFromFile(const QString &filename)
{
    delete map;
    map = new QImage(filename);
    return true;
}

QPointF CoreService::getBuildMapTargetPoint() const
{
    return buildMapTargetPoint;
}

QPointF CoreService::getBuildMapCurrentPoint() const
{
    return buildMapCurrentPoint;
}

qreal CoreService::distanceBetween(const QPointF &left, const QPointF &right)
{
    return sqrt(pow(left.x() - right.x(), 2) + pow(left.y() - right.y(), 2));
}

void CoreService::resetPositionRandomly(QPointF &target)
{
    target.rx() = rand() % MAX_WIDTH;
    target.ry() = rand() % MAX_HEIGHT;
}
