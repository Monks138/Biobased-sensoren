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

#define CONFIG_FILE "settings.ini"
#define CONFIG_VALUES_COUNT 3

WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, INFLUXDB_HOST, INFLUXDB_PORT);
InfluxDB *influxDB;

// sensors
Sensor *sensor;

String CONFIG_VALUES[] = {"WIFI-SSID", "WIFI-PASSWORD", "SENSOR-TYPE", "ROOM"};
SettingsInitializer settingsInitializer(CONFIG_VALUES,CONFIG_VALUES_COUNT, CONFIG_FILE);

void connectToWifi();
void checkSensorType();
arduino::String getMACAddressString();

void setup()
{
    Serial.begin(115200);

    while(!Serial){}

    pinMode(PIN_CD, INPUT);

    if (settingsInitializer.begin()) {
        Serial.println("Settings loaded successfully:");
        for (int i = 0; i < 2; i++) {
            Serial.print(CONFIG_VALUES[i] + ": ");
            Serial.println(settingsInitializer.getValue(CONFIG_VALUES[i]));
        }
    } else {
        Serial.println("Failed to load settings");
    }

    if(strcmp(settingsInitializer.getValue("SENSOR-TYPE"), "CO2") == 0) {
        sensor = new SCD30CO2();
    } else if(strcmp(settingsInitializer.getValue("SENSOR-TYPE"), "VOC") == 0) {
        sensor = new SGP30VOC();
    } else if(strcmp(settingsInitializer.getValue("SENSOR-TYPE"), "TEMPANDHUMIDITY") == 0) {
        sensor = new HDC1080();
    } else {
        Serial.println("Invalid sensor type");
        exit(1);
    }

    sensor->begin();

    connectToWifi();

    Serial.println("End of setup");
    Serial.println("Sensortype: " + arduino::String(settingsInitializer.getValue("SENSOR-TYPE")));

    influxDB = new InfluxDB(INFLUXDB_HOST, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
}

void loop()
{
    float measurement = sensor->measure();
    Serial.println("measured data, going to make point");

    Point point = Point().measurement("co2_sensor")
        .addTag("room", "badkamer_guus")
        .addTag("sensor_id", getMACAddressString().c_str())
        .addTag("unit", "ppm")
        .addField("co2", measurement);
    Serial.println("Data measured!");

    influxDB->writePoint(point, client);
    Serial.println("Data sent to InfluxDB!");
    delay(1000);
}

void connectToWifi() {
  int status = WL_IDLE_STATUS; 

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(arduino::String(settingsInitializer.getValue("WIFI-SSID")));
    status = WiFi.begin(settingsInitializer.getValue("WIFI-SSID"), settingsInitializer.getValue("WIFI-PASSWORD"));
    delay(10000);
  }
  Serial.println("Connected to wifi!");
}

arduino::String getMACAddressString() {
  byte mac[6];
  WiFi.macAddress(mac);

  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  return arduino::String(macStr);
}