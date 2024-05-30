#ifndef SGP30_SENSOR_H
#define SGP30_SENSOR_H

#include "Sensor.h"
#include <Adafruit_SGP30.h>

class SGP30 : public Sensor {
public:
    SGP30(uint8_t i2cAddress);
    void begin() override;
    // Measure TVOC (Total Volatile Organic Compounds) and return the value (ppb)
    float measure() override;
private:
    Adafruit_SGP30 sgp;
};

#endif
