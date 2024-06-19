#include "Log.h"

/**
 * @brief Log constructor.
 *
 * This constructor initializes the NTPClient object in the Log class.
 * It creates a new NTPClient object using the provided UDP socket and NTP server name.
 * The UTC offset for summer is also passed as a parameter to the NTPClient constructor.
 *
 * @param None
 * @return None
 */
Log::Log() {
    this->ntpClient = new NTPClient(*udpSocket, "pool.ntp.org", utcOffsetSummer);
}

/**
 * @brief Begin the connection with NTP server.
 *
 * This function initializes the NTP client and begins the connection with the NTP server.
 *
 * @note Make sure to set the NTP server before calling this function.
 *
 * @see setNTPServer()
 *
 * @return void
 */
void Log::beginConnection() {
    //this->ntpClient->begin();
}

/**
 * @class Log
 *
 * @brief The Log class is responsible for logging messages, performing network time synchronization,
 * and providing debug functionalities.
 *
 * This class manages a UDP socket connection and an NTP client for network time synchronization.
 * It provides methods for logging messages, setting debug mode, initiating the network connection,
 * and getting the single instance of the Log class.
 */
Log::~Log() {
    delete ntpClient;
    delete udpSocket;
}

/**
 * @class Log
 *
 * @brief The Log class provides a way to log information and error messages.
 *
 * The Log class is implemented as a singleton. It ensures that only one instance
 * of the Log class can be created and provides a global point of access to it.
 *
 * Example usage:
 * @code{.cpp}
 *   Log::getInstance().info("Information message");
 *   Log::getInstance().error("Error message");
 * @endcode
 */
Log& Log::getInstance() {
    static Log instance;
    return instance;
}

/**
 * @brief Set the debug mode for logging.
 *
 * This function sets the debug mode for logging. When debug mode is enabled, additional information
 * and debug messages will be logged.
 *
 * @param mode The debug mode value. Set to true to enable debug mode, false otherwise.
 *
 * @note The debug mode is disabled by default.
 */
void Log::setDebugMode(bool mode) {
    this->debugMode = mode;
}

/**
 * Log an informational message.
 *
 * @param message The message to be logged.
 */
void Log::info(const arduino::String &message) {
    char* level = "INFO";
    //if(WiFi.status() == WL_CONNECTED) {
   //     this->serialPrintTime();
    //}
    Serial.print(level);
    Serial.print(": ");
    Serial.println(message);
    writeToCard(level, message);
}

/**
 * @brief Logs an error message and writes it to the serial port and SD card (if debug mode is enabled).
 *
 * @param message The error message to log.
 */
void Log::error(const arduino::String &message) {
    char* level = "ERROR";
//    if(WiFi.status() == WL_CONNECTED) {
//        this->serialPrintTime();
//    } else {
//        Serial.print("[NO WIFI] ");
//    }
    Serial.print(level);
    Serial.print(": ");
    Serial.println(message);
    writeToCard(level, message);
}

/**
 * @brief Writes the log message to the SD card if debug mode is enabled.
 *
 * @param level The log level of the message.
 * @param message The log message.
 */
void Log::writeToCard(const arduino::String &level, const arduino::String &message) {
    if(this->debugMode) {
        File file = SD.open(fileName, FILE_WRITE);
        file.write("[");
    //    if (WiFi.status() == WL_CONNECTED) {
    //        file.write(ntpClient->getFormattedTime().c_str());
    //    } else {
    //        file.write("NO WIFI");
    //    }
        file.write("] ");
        file.write(level.c_str());
        file.write(": ");
        file.write(message.c_str());
        file.write("\n");
        file.flush();
        file.close();
    }
}

/**
 * @brief Prints the current time to the serial monitor.
 *
 * This function checks if the WiFi is connected and then updates the NTP client to get the current time.
 * The time is then printed in a formatted manner to the serial monitor.
 *
 * @note This function should be called when you want to log the time in your application.
 */
void Log::serialPrintTime() {
    if(WiFi.status() == WL_CONNECTED) {
        ntpClient->update();
        Serial.print("[");
        Serial.print(ntpClient->getFormattedTime());
        Serial.print("] ");
    }
}
