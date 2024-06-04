#include "SCD30CO2.h"

SCD30CO2::SCD30CO2(uint8_t i2cAddress) : Sensor(i2cAddress), scd30sensor()
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
    if (scd30sensor.isAvailable())
    {
        float result[3];
        scd30sensor.getCarbonDioxideConcentration(result);
        return result[0];
    }
    else
    {
        return -1.0;
    }

}
