#ifndef HDC1080_SENSOR_H
#define HDC1080_SENSOR_H

#include "Sensor.h"
#include <ClosedCube_HDC1080.h>

class HDC1080 : public Sensor
{
public:
    HDC1080(uint8_t i2cAddress);
    void begin() override;
    float measure() override;
    float humidity();

private:
    ClosedCube_HDC1080 hdc;
};

#endif