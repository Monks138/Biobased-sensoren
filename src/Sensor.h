#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>

class Sensor {
public:
    Sensor(uint8_t i2cAddress);
    virtual void begin() = 0;
    virtual float measure() = 0;

protected:
    uint8_t i2cAddr;
    float offset;
};

#endif
