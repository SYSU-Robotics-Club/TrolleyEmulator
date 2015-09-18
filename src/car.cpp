#include "car.h"
#include "coreservice.h"

#include <math.h>
#include <QDebug>

#define CAR_LENGTH 50
#define CAR_WIDTH 24
#define SENSOR_RADIUS 5

Car::Car() : carPosition(100, 100), carDirection(90), carSpeed(1)
{
    sensorsList.push_back(Sensor(-(CAR_WIDTH / 2), -(CAR_LENGTH / 2 + 2 * SENSOR_RADIUS), SENSOR_RADIUS));
    sensorsList.push_back(Sensor(CAR_WIDTH / 2, -(CAR_LENGTH / 2 + 2 * SENSOR_RADIUS), SENSOR_RADIUS));
}

Car::~Car()
{

}

void Car::carMoveFoward()
{
    carPosition.rx() += carSpeed * sin((carDirection / 360.0) * (M_PI * 2));
    carPosition.ry() -= carSpeed * cos((carDirection / 360.0) * (M_PI * 2));
}

void Car::drawComponents(QPainter *painter)
{
    painter->save();

    painter->translate(carPosition);
    painter->rotate(carDirection);

    painter->drawRoundedRect(-(CAR_WIDTH / 2), -(CAR_LENGTH / 2), CAR_WIDTH, CAR_LENGTH, 5, 5);

    foreach (Sensor s, sensorsList) {
        s.drawSelf(painter);
    }

    painter->restore();
}

void Car::turnRight()
{
    carDirection = (carDirection + 1) % 360;
}

void Car::turnLeft()
{
    carDirection = (carDirection + 360 - 1) % 360;
}

void Car::act()
{
    QTransform transform;
    transform.translate(carPosition.x(), carPosition.y());
    transform.rotate(carDirection);
    bool sensor1Detected = false;
    bool sensor2Detected = false;
    std::list<Sensor>::iterator iterater = sensorsList.begin();
    if (iterater != sensorsList.end()) {
        sensor1Detected = (*iterater).sensorDetect(transform);
        iterater++;
        if (iterater != sensorsList.end()) {
            sensor2Detected = (*iterater).sensorDetect(transform);
        }

    }

    if (!(sensor1Detected && sensor2Detected)) {
        if (sensor1Detected) {
            turnLeft();
        } else if (sensor2Detected) {
            turnRight();
        }
    }
    carMoveFoward();
}

void Car::addSensor(const Sensor &newSensor)
{
    sensorsList.push_back(newSensor);
}

void Car::removeLastSensor()
{
    if (sensorsList.size()) {
        sensorsList.pop_back();
    }
}
std::list<Sensor> Car::getSensorsList() const
{
    return sensorsList;
}


