#include <Arduino.h>
#include "SGP30VOC.h"
#include "SCD30CO2.h"
#include "SettingsInitializer.h"

SGP30VOC sgp30(0x58);
bool sgp30connected = true;

SCD30CO2 scd30sensor(0x61);
bool scd30connected = true;

#define CONFIG_FILE "settings.ini"
#define CONFIG_VALUES_COUNT 3
String CONFIG_VALUES[] = {"WIFI-SSID", "WIFI-PASSWORD", "SENSOR-TYPE"};
SettingsInitializer settingsInitializer(CONFIG_VALUES,CONFIG_VALUES_COUNT, CONFIG_FILE);

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello world!");

    if (settingsInitializer.begin()) {
        Serial.println("Settings loaded successfully:");
        for (int i = 0; i < 2; i++) {
            Serial.print(CONFIG_VALUES[i] + ": ");
            Serial.println(settingsInitializer.getValue(CONFIG_VALUES[i]));
        }
    } else {
        Serial.println("Failed to load settings");
    }
     
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