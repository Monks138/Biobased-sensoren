#include "SGP30.h"

SGP30::SGP30(uint8_t i2cAddress) : Sensor(i2cAddress), sgp() {
}

void SGP30::begin() {
    Wire.begin();
    if (!sgp.begin()) {
        Serial.println("SGP30 sensor not found :(");
        while (1);
    }
    Serial.println("SGP30 sensor found :)");

    offset = 0.0;

    // Start the air quality sensor with default baseline values
    sgp.IAQinit();
}

float SGP30::measure() {
    if (!sgp.IAQmeasure()) {
        Serial.println("Measurement failed");
        return NAN;
    }
    return sgp.TVOC + offset;
}
