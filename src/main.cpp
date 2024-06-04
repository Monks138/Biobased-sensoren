#include <Arduino.h>
#include "SGP30VOC.h"
#include "SCD30CO2.h"
#include "HDC1080.h"

SGP30VOC sgp30(0x58);
bool sgp30connected = false;

SCD30CO2 scd30sensor(0x61);
bool scd30connected = false;

HDC1080 hdc(0x00);
bool hdc1080connected = true;

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
    if (hdc1080connected)
    {
        hdc.begin();
    }
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
    if (hdc1080connected)
    {
        Serial.print("Temp: ");
        Serial.println(hdc.measure());
        Serial.print("Hum: ");
        Serial.print(hdc.humidity());
        Serial.println(" %");
    }

    delay(2000);
}