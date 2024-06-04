#ifndef SCD30CO2_SENSOR_H
#define SCD30CO2_SENSOR_H

#include "Sensor.h"
#include <SCD30.h>

class SCD30CO2 : public Sensor
{
public:
    SCD30CO2(uint8_t i2cAddress);
    void begin() override;
    // Measure CO2 and return the value (ppm)
    float measure() override;

private:
    SCD30 scd30sensor;
};

#endif
