#include <Arduino.h>

#include "SGP30VOC.h"
#include "SCD30CO2.h"
#include "HDC1080.h"
#include "SettingsInitializer.h"
#include <SD.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "InfluxDB.h"

#define PIN_SPI_CS 4
#define PIN_CD 7
#define SETTINGS_FILE "settings.ini"
#define SEPERATOR_INDEX '='

#define INFLUXDB_HOST "influx-playground.sendlab.nl"
#define INFLUXDB_ORG "Sendlab"
#define INFLUXDB_PORT 443
#define INFLUXDB_BUCKET "STU-TI-Biobased-2024"
#define INFLUXDB_TOKEN "UCTCPxVZVeYbQJMcuNcuXW9tf-KhKn90zrQNBN-tddnLjnKrBrsYKkt-scqPx5N2nwvcghdpYchs638xOviHTA=="

char wifiSSID[30];
char wifiPassword[30];
char sensorType[30];

WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, INFLUXDB_HOST, INFLUXDB_PORT);
InfluxDB influxDB = InfluxDB(INFLUXDB_HOST, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

// sensors
SGP30VOC sgp30(0x58);
bool sgp30connected = false;

SCD30CO2 scd30sensor(0x61);
bool scd30connected = false;

HDC1080 hdc(0x00);
bool hdc1080connected = true;

void setup()
{
    Serial.begin(115200);

    while(!Serial){}

    pinMode(PIN_CD, INPUT);

    while(!SD.begin(PIN_SPI_CS)) {
        Serial.println("SD CARD FAILED, OR NOT PRESENT!");
        delay(1000);
    }

    loadFromSdCard();

    if(!wifiSSID || !wifiPassword) {
        Serial.println("WIFI Credentials not found. stopping...");
        return;
    }

    connectToWifi();



    Point point = Point().measurement("co2_sensor")
        .addTag("room", "badkamer_guus")
        .addTag("sensor_id", "00-00-00-00-00-03")
        .addTag("unit", "ppm")
        .addField("co2", 2.0);

    influxDB.writePoint(point, client);
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

void connectToWifi() {
  int status = WL_IDLE_STATUS; 

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(wifiSSID);
    status = WiFi.begin(wifiSSID, wifiPassword);
    delay(10000);
  }
}

void loadFromSdCard() {
    File envFile = SD.open(SETTINGS_FILE);
    if (envFile) {
        // Lees het bestand regel voor regel
        while (envFile.available()) {
        String line = envFile.readStringUntil('\n');

        int separatorIndex = line.indexOf(SEPERATOR_INDEX);
        if (separatorIndex != -1) {
            String key = line.substring(0, separatorIndex);
            String value = line.substring(separatorIndex + 1);

            key.trim();
            value.trim();
            if (key == "WIFI-SSID") {
                value.toCharArray(wifiSSID, sizeof(wifiSSID));
            } else if (key == "WIFI-PASSWORD") {
                value.toCharArray(wifiPassword, sizeof(wifiPassword));
            } else if (key == "SENSOR-TYPE") {
                value.toCharArray(sensorType, sizeof(sensorType));
                }
            }
        }
    envFile.close();
    } else {
        Serial.print("Fout bij het openen van ");
        Serial.println(SETTINGS_FILE);
    }
}