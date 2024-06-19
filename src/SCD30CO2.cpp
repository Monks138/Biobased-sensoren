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

    offset = OFFSET;
}

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
