#ifndef HDC1080_SENSOR_H
#define HDC1080_SENSOR_H

#include "Sensor.h"
#include <ClosedCube_HDC1080.h>

class HDC1080 : public Sensor
{
public:
    HDC1080();
    void begin() override;
    float measure() override;
    float humidity();
    SensorPoint getMeasurementPoints(char* room, char* macAddress) override;

private:
    ClosedCube_HDC1080 hdc;
};

#endif