#include "Statusmanager.h"
#include <Arduino.h>


#define STATUS_LED_RED 2
#define STATUS_LED_GREEN 5
#define STATUS_LED_BLUE 6

#define ERROR_WAIT_TIME 1000 * 60 // 1 min
#define GREEN_LED_TIME 1000 * 60 // 1 min

// Private constructor definitie
StatusManager::StatusManager() : _rgbLed(STATUS_LED_RED, STATUS_LED_GREEN, STATUS_LED_BLUE) {
    Serial.println("StatusManager initialized.");
    setStatus(Colors::Blue);
}

// Singleton getInstance methode
StatusManager& StatusManager::getInstance() {
    static StatusManager instance;
    return instance;
}

void StatusManager::setStatus(const Color& color) {
    _rgbLed.setColor(color);
}

void StatusManager::error(String error, const Color& color, int errorCode) {
    Serial.print("Error Called:");
    Serial.println(error);


    unsigned long currentTime = millis();
    unsigned long currentTimeInSeconds = currentTime / 1000;

    Serial.print("Application ran for ");
    Serial.print(currentTimeInSeconds);
    Serial.println(" Seconds");

    unsigned long secs = ERROR_WAIT_TIME / 1000;
    Serial.print("Exiting in: ");
    Serial.print(secs);
    Serial.println(" Seconds");


    _rgbLed.setColor(color);
    delay(ERROR_WAIT_TIME);
    _rgbLed.setColor(Colors::Off);
    Serial.println("Exited...");
    exit(errorCode);
}

void StatusManager::setCurrentTime() {
    statusGreenTime = millis();
}

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
