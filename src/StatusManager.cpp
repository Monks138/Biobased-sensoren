#include "Statusmanager.h"
#include <Arduino.h>
#include "Log.h"


#define STATUS_LED_RED 2
#define STATUS_LED_GREEN 5
#define STATUS_LED_BLUE 6

#define ERROR_WAIT_TIME 1000 * 60 // 1 min
#define GREEN_LED_TIME 1000 * 60 // 1 min

/**
 * @class StatusManager
 * @brief Manages the status of the system using an RGB LED.
 *
 * The StatusManager class is responsible for managing the status of the system.
 * It provides methods to set the status color, handle errors, and update the status.
 */
// Private constructor definitie
StatusManager::StatusManager() : _rgbLed(STATUS_LED_RED, STATUS_LED_GREEN, STATUS_LED_BLUE) {
    Serial.println("StatusManager initialized.");
    setStatus(Colors::Blue);
}

/**
 * @class StatusManager
 * @brief The StatusManager class provides a singleton instance for controlling the status and lighting of an RGB LED.
 *
 * The StatusManager class is designed to have only one instance throughout the application. The getInstance() method
 * returns the same instance of the StatusManager class in all invocations.
 */
// Singleton getInstance methode
StatusManager& StatusManager::getInstance() {
    static StatusManager instance;
    return instance;
}

/**
 * @brief The StatusManager class handles the status of the RGB LED.
 *
 * This class provides methods to set the status of the RGB LED by setting its color.
 */
void StatusManager::setStatus(const Color& color) {
    _rgbLed.setColor(color);
}

/**
 * @brief The StatusManager class handles error management and status updates.
 */
void StatusManager::error(String error, const Color& color, int errorCode) {
    Log::getInstance().error("Error Called" + error);


    unsigned long currentTime = millis();
    unsigned long currentTimeInSeconds = currentTime / 1000;

    Log::getInstance().error("Application ran for " + String(currentTimeInSeconds) + " Seconds");

    unsigned long secs = ERROR_WAIT_TIME / 1000;
    Log::getInstance().error("Exiting in: " + String(secs) + " Seconds");


    _rgbLed.setColor(color);
    delay(ERROR_WAIT_TIME);
    _rgbLed.setColor(Colors::Off);
    Log::getInstance().error("Exited...");
    exit(errorCode);
}

/**
 * @brief Sets the current time to the statusGreenTime variable.
 *
 * This function updates the statusGreenTime variable with the current time in milliseconds.
 *
 * @note The statusGreenTime variable is defined as -1 by default.
 *
 * @see statusGreenTime
 */
void StatusManager::setCurrentTime() {
    statusGreenTime = millis();
}

/**
 * @brief Updates the status of the StatusManager.
 *
 * This function is responsible for updating the status of the StatusManager. If the successBooted flag is already set to true,
 * the function returns immediately. Otherwise, it checks if the statusGreenTime is not equal to -1. If so, it checks if the
 * difference between the current time and statusGreenTime is greater than or equal to GREEN_LED_TIME. If the condition is true,
 * it sets the color of the RGB LED to off and sets the successBooted flag to true.
 *
 * @note This function assumes that the _rgbLed object is properly initialized and available for use.
 *
 * @param None
 *
 * @return None
 */
void StatusManager::update() {
    if(this->successBooted) {
        return;
    }
    if(this->statusGreenTime != -1) {
        if (millis() - statusGreenTime >= GREEN_LED_TIME) {
            _rgbLed.setColor(Colors::Off);
            successBooted = true;
        }
    }
}
