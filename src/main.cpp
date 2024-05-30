#include <Arduino.h>
#include "SGP30.h"

SGP30 sgp30(0x58);
bool sgp30connected = true;

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello world!");
     
    if (sgp30connected)
    {
        sgp30.begin();
    }
    Serial.println(sgp30.measure());
}

void loop()
{
    if (sgp30connected)
    {
        Serial.println(sgp30.measure());
    }

    delay(1000);
}