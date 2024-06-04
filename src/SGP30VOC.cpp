#include "SGP30VOC.h"

SGP30VOC::SGP30VOC(uint8_t i2cAddress) : Sensor(i2cAddress), sgp() {
}

void SGP30VOC::begin() {
    Wire.begin();
    if (!sgp.begin()) {
        Serial.println("SGP30 sensor not found :(");
        return;
    }
    Serial.println("SGP30 sensor found :)");

    offset = 0.0;

    // Start the air quality sensor with default baseline values
    sgp.IAQinit();
}

float SGP30VOC::measure() {
    if (!sgp.IAQmeasure()) {
        return -1.0;
    }
    return sgp.TVOC + offset;
}
