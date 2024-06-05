#include "SCD30CO2.h"

SCD30CO2::SCD30CO2() : Sensor(), scd30sensor()
{
}

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

    offset = 0.0;
}

float SCD30CO2::measure()
{
    if (!scd30sensor.isAvailable())
    {
        return -1.0;
    }
    float result[3];
    scd30sensor.getCarbonDioxideConcentration(result);
    return result[0] + offset;
}

Point* SCD30CO2::getMeasurementPoints(char* room, char* macAddress)
{
    float measurement = measure();
    Point point = Point().measurement("co2_sensor")
        .addTag("room", room)
        .addTag("sensor_id", macAddress)
        .addTag("unit", "ppm")
        .addField("co2", measurement);

        return nullptr;
}
