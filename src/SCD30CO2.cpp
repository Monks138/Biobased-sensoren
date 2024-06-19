#include "SCD30CO2.h"

/**
 * @class SCD30CO2
 * @brief The SCD30CO2 class represents a CO2 sensor based on the SCD30 module.
 *
 * This class provides methods to start and measure CO2 levels using the SCD30 sensor.
 */
SCD30CO2::SCD30CO2() : Sensor(), scd30sensor()
{
}

/**
 * @brief Initializes the SCD30CO2 sensor.
 *
 * This function initializes the required dependencies and checks if the sensor is available.
 * If the sensor is not available, an error message is printed, and the function returns.
 * If the sensor is available, a success message is printed.
 */
void SCD30CO2::begin()
{
    Wire.begin();
    scd30sensor.initialize();
    if (!scd30sensor.isAvailable())
    {
        Serial.println("SCD30CO2 sensor not found :(");
        return;
    }
    Serial.println("SCD30CO2 sensor found :)");

    offset = OFFSET;
}

/**
 * @brief Measure the CO2 concentration using the SCD30 sensor and return the value
 *
 * @return float - The measured CO2 concentration in parts per million (ppm)
 *                If the sensor is not available, returns -1.0
 *                If the calculated CO2 concentration is not within the valid range (0 to 20000 ppm), returns -1.0
 */
float SCD30CO2::measure()
{
    if (!scd30sensor.isAvailable())
    {
        return -1.0;
    }
    float result[3];
    scd30sensor.getCarbonDioxideConcentration(result);

    float co2 = result[0] + offset;

    if(co2 < 0 || co2 > 20000)
    {
        -1.0;
    }
    return co2;
}

/**
 * @brief Calculates the measurement points for a CO2 sensor.
 *
 * This function creates a SensorPoint object and initializes it with the measurement points for a CO2 sensor.
 * It takes in the room name and MAC address as parameters.
 *
 * @param room The name of the room where the measurement is taken.
 * @param macAddress The MAC address of the sensor.
 * @return A SensorPoint object containing the measurement points.
 */
SensorPoint SCD30CO2::getMeasurementPoints(const char* room, char* macAddress)
{
    SensorPoint sensorPoint = SensorPoint();
    short size = 1;

    sensorPoint.size = size;
    sensorPoint.points = new Point[size];

    sensorPoint.points[0] = Point().measurement("co2_sensor")
            .addTag("room", room)
            .addTag("sensor_id", macAddress)
            .addTag("unit", "ppm")
            .addField("co2", this->measure());
    Serial.println("Data measured!");


    return sensorPoint;
}
