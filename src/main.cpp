#include <Arduino.h>
#include "SGP30VOC.h"
#include "SCD30CO2.h"

SGP30VOC sgp30(0x58);
bool sgp30connected = true;

SCD30CO2 scd30sensor(0x61);
bool scd30connected = true;

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello world!");
     
    if (sgp30connected)
    {
        sgp30.begin();
    }
    if (scd30connected)
    {
        scd30sensor.begin();
    }
    Serial.println(sgp30.measure());
}

void loop()
{
    Serial.println("Looping...");
    if (sgp30connected)
    {
        Serial.print("sgp: ");
        Serial.println(sgp30.measure());
    }
    if (scd30connected)
    {
        Serial.print("scd: ");
        Serial.println(scd30sensor.measure());
    }

    delay(2000);
}