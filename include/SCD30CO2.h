#ifndef SCD30CO2_SENSOR_H
#define SCD30CO2_SENSOR_H

#include "Sensor.h"
#include <SCD30.h>

#define OFFSET -100

class SCD30CO2 : public Sensor
{
public:
    SCD30CO2();
    void begin() override;
    // Measure CO2 and return the value (ppm)
    float measure() override;
    SensorPoint getMeasurementPoints(const char* room, char* macAddress) override;

private:
    SCD30 scd30sensor;
};

#endif
