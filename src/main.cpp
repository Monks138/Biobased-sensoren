#include <Arduino.h>

#include "SGP30VOC.h"
#include "SCD30CO2.h"
#include "HDC1080.h"
#include "SettingsInitializer.h"
#include <SD.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include "InfluxDB.h"
#include "StatusManager.h"
#include "Log.h"

#define PIN_CD 7

#define INFLUXDB_HOST "influx-playground.sendlab.nl"
#define INFLUXDB_ORG "Sendlab"
#define INFLUXDB_PORT 443
#define INFLUXDB_BUCKET "STU-TI-Biobased-2024"
#define INFLUXDB_TOKEN "UCTCPxVZVeYbQJMcuNcuXW9tf-KhKn90zrQNBN-tddnLjnKrBrsYKkt-scqPx5N2nwvcghdpYchs638xOviHTA=="

#define CONFIG_FILE "settings.ini"
#define CONFIG_VALUES_COUNT 5


WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, INFLUXDB_HOST, INFLUXDB_PORT);
InfluxDB *influxDB;

// sensors
Sensor *sensor;

String CONFIG_VALUES[] = {"WIFI-SSID", "WIFI-PASSWORD", "SENSOR-TYPE", "ROOM", "UPDATE-TIME"};
SettingsInitializer settingsInitializer(CONFIG_VALUES,CONFIG_VALUES_COUNT, CONFIG_FILE);

void connectToWifi();
char* getMACAddressString();

void setup()
{
    StatusManager::getInstance();
    delay(5000);

    Serial.begin(115200);

// Only for debugging
//    while(!Serial){}

    settingsInitializer.begin();
    Log::getInstance().info("Settings loaded successfully");
    for (const auto & i : CONFIG_VALUES) {
        Log::getInstance().info(i + ": " + settingsInitializer.getValue(i));
    }

    if(strcmp(settingsInitializer.getValue("SENSOR-TYPE"), "CO2") == 0) {
        sensor = new SCD30CO2();
        Log::getInstance().info("Sensor type: CO2");
    } else if(strcmp(settingsInitializer.getValue("SENSOR-TYPE"), "VOC") == 0) {
        sensor = new SGP30VOC();
        Log::getInstance().info("Sensor type: VOC");
    } else if(strcmp(settingsInitializer.getValue("SENSOR-TYPE"), "TEMPANDHUMIDITY") == 0) {
        sensor = new HDC1080();
        Log::getInstance().info("Sensor type: TEMPANDHUMIDITY");
    } else {
        StatusManager::getInstance().error("SensorType not found (that was stated in the config)", Colors::Cyan);
        Log::getInstance().error("SensorType not found (that was stated in the config)");
    }

    sensor->begin();

    Log::getInstance().info("Sensortype" + arduino::String(settingsInitializer.getValue("SENSOR-TYPE")));
    // Serial.println("Sensortype: " + arduino::String(settingsInitializer.getValue("SENSOR-TYPE")));

    connectToWifi();


    // Serial.println("End of setup");
    // Serial.println("Sensortype: " + arduino::String(settingsInitializer.getValue("SENSOR-TYPE")));

    influxDB = new InfluxDB(INFLUXDB_HOST, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

    StatusManager::getInstance().setStatus(Colors::Green);
    StatusManager::getInstance().setCurrentTime();

    Log::getInstance();
    Log::getInstance().info("End of setup");
}

void loop()
{
    StatusManager::getInstance().update();
    if(WiFi.status() != 3) {
        connectToWifi();
    }
    SensorPoint point = sensor->getMeasurementPoints(settingsInitializer.getValue("ROOM"), getMACAddressString());

    int statusCode = -1;
    if(point.size > 1) {
        statusCode = influxDB->writePoints(point.points, point.size, client);
    } else if(point.size == 1){
        statusCode = influxDB->writePoint(point.points[0], client);
    }

    delete point.points;

    if(statusCode != 204) {
        StatusManager::getInstance().error("Could not write to InfluxDB", Colors::Red);
    }

    String updateTime = settingsInitializer.getValue("UPDATE-TIME");
    int updateTimeInMilli = updateTime.toInt();


    Serial.println("Status: " + String(WiFi.status()));
    delay(updateTimeInMilli);
}

void connectToWifi() {
  int status = WL_IDLE_STATUS;

  int count = 0;

  while (status != WL_CONNECTED) {
    Log::getInstance().info("Attempting to connect to network: " + arduino::String(settingsInitializer.getValue("WIFI-SSID")));
    // Serial.print("Attempting to connect to network: ");
    // Serial.println(arduino::String(settingsInitializer.getValue("WIFI-SSID")));
    status = WiFi.begin(settingsInitializer.getValue("WIFI-SSID"), settingsInitializer.getValue("WIFI-PASSWORD"));
    delay(10000);
    count++;
    if(count >= 10) {
        break;
    }
  }
  if(status != WL_CONNECTED) {
      StatusManager::getInstance().error("Could not connect to WiFi", Colors::Orange);
  }

  Log::getInstance().info("Connected to wifi!");
//   Serial.println("Connected to wifi!");
}

char* getMACAddressString() {
//  byte mac[6];
//  WiFi.macAddress(mac);
//
//  char macStr[18];
//  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
//
//  return macStr;
    return "test-mac";

}