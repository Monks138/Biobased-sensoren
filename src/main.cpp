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
#include <Adafruit_SleepyDog.h>

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

/**
 * @brief Connects to a WiFi network.
 *
 * Attempts to connect to a specified WiFi network using the SSID and password
 * obtained from the settingsInitializer. If the connection is successful, the
 * function logs a message indicating the successful connection. If the connection
 * fails, an error message is logged.
 *
 * @note This function uses the Watchdog to reset the device while attempting to connect
 * and logs messages using the Log and StatusManager classes.
 *
 * @see WL_IDLE_STATUS
 * @see WL_CONNECTED
 * @see Watchdog
 * @see Log
 * @see StatusManager
 * @see settingsInitializer
 */
void connectToWifi();
/**
 * \fn char* getMACAddressString()
 * \brief Retrieves the MAC address of the device and converts it to a string.
 *
 * This function retrieves the MAC (Media Access Control) address of the device and converts it to a string representation.
 * The MAC address is a unique identifier assigned to the network interface of the device.
 *
 * \return A pointer to a character array representing the MAC address string.
 *
 * \details This function uses the WiFi.macAddress() method to retrieve the MAC address of the device. It then converts the MAC address
 * to a string representation using the sprintf() function and stores it in the static char array macStr. Finally, it returns a pointer
 * to the macStr array.
 *
 * \note The returned MAC address string is in the format "XX:XX:XX:XX:XX:XX" where X represents a hexadecimal digit.
 *
 * \see WiFiClass::macAddress()
 */
char* getMACAddressString();


// Function to calculate free memory
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;+
#endif  // __arm__

/**
 * @brief Calculates the amount of free memory in bytes.
 *
 * The function measures the amount of free memory available on the system by
 * determining the difference between the current stack pointer and the heap
 * memory location.
 *
 * @return The amount of free memory in bytes.
 */
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

unsigned long currentMillis;

/**
 * @brief Initialize the program
 *
 * This function sets up the necessary components and configurations for the program to run.
 *
 * @note Requires the following definitions:
 * - Serial: a Serial object
 * - StatusManager: a StatusManager class instance
 * - Log: a Log class instance
 * - settingsInitializer: a SettingsInitializer object
 * - CONFIG_VALUES: an array of Strings containing configuration values
 * - CONFIG_VALUES_COUNT: the number of elements in CONFIG_VALUES array
 * - CONFIG_FILE: a String specifying the configuration file
 * - Sensor: a base class for different types of sensors
 * - SCD30CO2: a class representing SCD30CO2 sensor
 * - SGP30VOC: a class representing SGP30VOC sensor
 * - HDC1080: a class representing HDC1080 sensor
 * - Colors: a namespace containing Color objects
 * - INFLUXDB_HOST: a char pointer representing InfluxDB host
 * - INFLUXDB_ORG: a char pointer representing InfluxDB organization
 * - INFLUXDB_BUCKET: a char pointer representing InfluxDB bucket
 * - INFLUXDB_TOKEN: a char pointer representing InfluxDB token
 * - WiFi: an external WiFiClass object
 * - Watchdog: an external WatchdogType object
 *
 * @note Requires the following functions:
 * - void connectToWifi(): connects to WiFi network
 * - void delay(unsigned long milliseconds): pauses the program for the specified number of milliseconds
 * - char* arduino::String::c_str(): returns a pointer to the underlying C-style character array of the String object
 * - int strcmp(const char* s1, const char* s2): compares two strings and returns 0 if they are equal
 * - void Sensor::begin(): initializes the sensor
 * - void Log::info(const String& message): logs an informational message
 * - void Log::error(const String& message): logs an error message
 * - void StatusManager::error(const String& message, const Color& color): logs an error message with the specified color
 * - void StatusManager::setStatus(const Color& color): sets the status color
 * - void StatusManager::setCurrentTime(): sets the current time in StatusManager
 * - String SettingsInitializer::getValue(const String& key): retrieves the value for the specified key from the configuration file
 * - InfluxDB::InfluxDB(char* influxdb_url, char* influxdb_org, char* influxdb_bucket, char* influxdb_token): constructor for InfluxDB class
 * - int SettingsInitializer::begin(): initializes the settings from the configuration file
 * - int WatchdogType::enable(int timeout): enables the Watchdog with the specified timeout in milliseconds and returns the countdown value
 *
 * @note Provides the following variables:
 * - Sensor* sensor: a pointer to the current sensor
 * - InfluxDB* influxDB: a pointer to the InfluxDB object
 * - int updateTimeInMilli: the update time in milliseconds
 * - const char* room: the room name
 * - unsigned long currentMillis: the current time in milliseconds
 */
void setup()
{
    Serial.begin(115200);
    StatusManager::getInstance();
    delay(500);

    Log::getInstance().info("Program Started!");

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

    String updateTime = settingsInitializer.getValue("UPDATE-TIME");
    updateTimeInMilli = updateTime.toInt();

    room = settingsInitializer.getValue("ROOM");

    int countdownMS = Watchdog.enable(10000);
    Log::getInstance().info("Watchdog started for " + String(countdownMS) + " millis");

    Log::getInstance().info("End of setup");
    currentMillis = 0;
}


/**
 * @brief Main loop function
 *
 * This function is called repeatedly in the main loop. It performs the following tasks:
 * 1. Resets the watchdog timer
 * 2. Retrieves the current time in milliseconds
 * 3. Checks if it's time to update the measurements
 * 4. Logs the current free memory
 * 5. Updates the status manager
 * 6. Logs the WiFi status
 * 7. Retrieves the sensor measurements and logs them
 * 8. Resets the watchdog timer
 * 9. Checks if all CO2 values are valid
 * 10. Writes the measurements to the InfluxDB database
 * 11. Logs the status of the write operation
 * 12. Deletes the measurement points array
 * 13. Checks if the write operation was successful
 * 14. Updates the current time
 * 15. Delays for 100 milliseconds
 */
void loop()
{
    Watchdog.reset();
    unsigned long newMillis = millis();

    if(newMillis - currentMillis >= updateTimeInMilli) {
        Log::getInstance().info("Free memory: " + String(freeMemory()) + " bytes");

        StatusManager::getInstance().update();
        Log::getInstance().info("WiFi Status: " + String(WiFi.status()));
        Log::getInstance().info("Retrieving data from sensors");
        SensorPoint point = sensor->getMeasurementPoints(room, getMACAddressString());
        Log::getInstance().info("Measurements received");

        Watchdog.reset();

        // Check if all values are valid for CO2
        if(arduino::String(settingsInitializer.getValue("SENSOR-TYPE")) == "CO2") {
            for(int i = 0; i < point.size; ++i) {
                int index = point.points[i].toLineProtocol().indexOf(" co2=");
                int end = point.points[i].toLineProtocol().length();
                String measurement = point.points[i].toLineProtocol().substring(index, end);

                if(measurement.indexOf("-1.0") < 0) {
                    Log::getInstance().info("Valid value found: " + measurement);
                } else {
                    Log::getInstance().error("Invalid value found, delaying for 20 secoonds to activate watchdog, measurement: " + measurement);
                    StatusManager::getInstance().setStatus(Colors::Purple);
                    delay(5000);
                    Watchdog.enable(1);
                    while(1);
                }
            }
        }

        int statusCode = -1;
        Log::getInstance().info("Writing points");
        if (WiFi.status() == WL_CONNECTED) {
            if (point.size > 1) {
                statusCode = influxDB->writePoints(point.points, point.size, client);
            } else if (point.size == 1) {

                statusCode = influxDB->writePoint(point.points[0], client);
            }
            Log::getInstance().info("Points written");
        } else {
            Log::getInstance().error("WiFi disconnected! Reconnecting...");
            connectToWifi();
        }

        delete[] point.points;
        point.points = nullptr;

        if (statusCode != 204) {
            Log::getInstance().error("Could not write to InfluxDB");
        }
    currentMillis = newMillis;
    }
    delay(100);
}

/**
 * @brief Connects to WiFi network
 *
 * This function attempts to connect to the specified WiFi network using the provided SSID and password.
 * It will keep trying to connect until it is successful or until a maximum number of attempts is reached.
 * If the connection is successful, it will log the success message. If the connection fails, it will log an error message.
 *
 * @note Requires the following definitions:
 * - WL_IDLE_STATUS: 0
 * - WL_CONNECTED
 * - Watchdog: an external WatchdogType object
 * - Log: a Log class instance
 * - settingsInitializer: a SettingsInitializer object
 * - WiFi: an external WiFiClass object
 * - StatusManager: a StatusManager class instance
 * - Orange: a Color object with RGB values (255, 165, 0)
 *
 * @note Requires the following functions:
 * - String arduino::String(const char *str)
 * - WiFi.begin(const char *ssid, const char *password)
 * - delay(unsigned long milliseconds)
 * - StatusManager::getInstance(): returns a reference to a StatusManager object
 * - Log::getInstance(): returns a reference to a Log object
 * - Log::info(const String &message): logs an informational message
 * - StatusManager::error(const String &message, const Color &color): logs an error message with the specified color
 */
void connectToWifi() {
  int status = WL_IDLE_STATUS;

  int count = 0;

  while (status != WL_CONNECTED) {
    Watchdog.reset();
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
    Watchdog.reset();
}

/**
 * @brief Get the MAC address as a string
 *
 * This function retrieves the MAC address of the WiFi module and converts it into a string format.
 * The MAC address is a unique identifier assigned to the network interface of the device.
 *
 * @return A pointer to a character array containing the MAC address string.
 *
 * @note The returned string is statically allocated and should not be modified or freed by the caller.
 */
char* getMACAddressString() {
 byte mac[6];
 WiFi.macAddress(mac);

 static char macStr[18];
 sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

 return macStr;
}

