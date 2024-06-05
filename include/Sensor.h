#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "Point.h"
#include "WiFiNINA.h"

class Sensor {
public:
    Sensor();
    virtual void begin() = 0;
    virtual float measure() = 0;
    virtual Point* getMeasurementPoints(char* room, char* macAddress) = 0;


protected:
    float offset;
};

#endif
