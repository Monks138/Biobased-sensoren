#include "SGP30VOC.h"

/**
 * @class SGP30VOC
 * @brief SGP30VOC class represents a VOC (Volatile Organic Compounds) sensor.
 *
 * This class inherits from the Sensor class and provides functionality to measure TVOC (Total Volatile Organic Compounds).
 * It uses the Adafruit_SGP30 library to communicate with the sensor and obtain measurement values.
 */
SGP30VOC::SGP30VOC() : Sensor(), sgp() {
}

/**
 * @class SGP30VOC
 * @brief Wrapper class for the SGP30 VOC (Volatile Organic Compounds) sensor
 */
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

/**
 * @defgroup SGP30VOC SGP30VOC
 * @brief SGP30VOC class is responsible for measuring the TVOC (Total Volatile Organic Compounds) value.
 * @{
 */
float SGP30VOC::measure() {
    if (!sgp.IAQmeasure()) {
        return -1.0;
    }
    return sgp.TVOC + offset;
}

/**
 * @brief getMeasurementPoints function returns measurement points for a given room and MAC address.
 *
 * The function takes a room name and MAC address as input parameters and returns a SensorPoint structure
 * containing an array of Point structures and its size. The SensorPoint structure represents the measurement
 * points obtained by the SGP30VOC sensor.
 *
 * @param room The room name for which measurement points are required
 * @param macAddress The MAC address of the device for which measurement points are required
 * @return SensorPoint A structure containing measurement points obtained by the SGP30VOC sensor
 */
SensorPoint SGP30VOC::getMeasurementPoints(const char* room, char* macAddress){
    SensorPoint sensorPoint;

    return sensorPoint;
}