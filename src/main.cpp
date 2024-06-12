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
#define CONFIG_VALUES_COUNT 6


WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, INFLUXDB_HOST, INFLUXDB_PORT);
InfluxDB *influxDB;

int updateTimeInMilli;
const char* room;

// sensors
Sensor *sensor;

String CONFIG_VALUES[] = {"WIFI-SSID", "WIFI-PASSWORD", "SENSOR-TYPE", "ROOM", "UPDATE-TIME", "DEBUG-MODE"};
SettingsInitializer settingsInitializer(CONFIG_VALUES,CONFIG_VALUES_COUNT, CONFIG_FILE);

void connectToWifi();
char* getMACAddressString();


// Function to calculate free memory
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;+
#endif  // __arm__

int freeMemory() {
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

void setup()
{
    Serial.begin(115200);
    StatusManager::getInstance();
    delay(5000);

// Only for debugging
//    while(!Serial){}

    settingsInitializer.begin();

    arduino::String debugMode = settingsInitializer.getValue("DEBUG-MODE");
    debugMode.toUpperCase();

    if(strcmp(debugMode.c_str(), "TRUE") == 0) {
        Log::getInstance().setDebugMode(true);
    } else if(strcmp(debugMode.c_str(), "FALSE") == 0) {
        Log::getInstance().setDebugMode(false);
    } else {
        Log::getInstance().error("Debug-Mode not found (that was stated in the config)");
    }

    Log::getInstance().info("Program Started!");
    Log::getInstance().info("Settings loaded successfully");
    for (const auto & i : CONFIG_VALUES) {
        Log::getInstance().info(i + ": " + settingsInitializer.getValue(i));
    }

    if(strcmp(settingsInitializer.getValue("SENSOR-TYPE"), "CO2") == 0) {
        sensor = new SCD30CO2();
    } else if(strcmp(settingsInitializer.getValue("SENSOR-TYPE"), "VOC") == 0) {
        sensor = new SGP30VOC();
    } else if(strcmp(settingsInitializer.getValue("SENSOR-TYPE"), "TEMPANDHUMIDITY") == 0) {
        sensor = new HDC1080();
    } else {
        StatusManager::getInstance().error("SensorType not found (that was stated in the config)", Colors::Cyan);
        Log::getInstance().error("SensorType not found (that was stated in the config)");
    }

    sensor->begin();

    Log::getInstance().info("Sensortype" + arduino::String(settingsInitializer.getValue("SENSOR-TYPE")));

    connectToWifi();

    influxDB = new InfluxDB(INFLUXDB_HOST, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

    StatusManager::getInstance().setStatus(Colors::Green);
    StatusManager::getInstance().setCurrentTime();

    Log::getInstance();
    Log::getInstance().info("End of setup");

    String updateTime = settingsInitializer.getValue("UPDATE-TIME");
    updateTimeInMilli = updateTime.toInt();

    room = settingsInitializer.getValue("ROOM");
}

void loop()
{
    Log::getInstance().info("Free memory: " + String(freeMemory()) + " bytes");

    StatusManager::getInstance().update();
    Log::getInstance().info("WiFi Status: " + String(WiFi.status()));
    if(WiFi.status() != 3) {
        Log::getInstance().error("WiFi disconnected! Reconnecting...");
        connectToWifi();
    }
    Log::getInstance().info("Retrieving data from sensors");
    SensorPoint point = sensor->getMeasurementPoints(room, getMACAddressString());
    Log::getInstance().info("Measurements received");

    int statusCode = -1;
    Log::getInstance().info("Writing points");
    if(point.size > 1) {
        statusCode = influxDB->writePoints(point.points, point.size, client);
    } else if(point.size == 1){
        statusCode = influxDB->writePoint(point.points[0], client);
    }
    Log::getInstance().info("Points written");

    delete[] point.points;
    point.points = nullptr;

    if(statusCode != 204) {
        Log::getInstance().error("Could not write to InfluxDB");
    }
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
//  Serial.println("WIFI Connected");
//  delay(1000);
//  Log::getInstance().beginConnection();
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

