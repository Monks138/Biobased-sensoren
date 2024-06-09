#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "Point.h"
#include "WiFiNINA.h"

struct SensorPoint {
    Point* points;
    short size;
};

class Sensor {
public:
    Sensor();
    virtual void begin() = 0;
    virtual float measure() = 0;
    virtual SensorPoint getMeasurementPoints(char* room, char* macAddress) = 0;


protected:
    float offset;
};



#endif
