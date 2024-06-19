//
// Created by Merijn on 6/4/2024.
//
#include "SettingsInitializer.h"
#include "Color.h"
#include "StatusManager.h"
#include <SPI.h>
#include <SD.h>
#include "Log.h"

#define SD_CS_PIN 4  // Chip select pin for SD card module

/**
 * @class SettingsInitializer
 *
 * @brief A class for initializing settings with given keys, count, and file path.
 *
 * The SettingsInitializer class allows you to initialize settings using a list of keys,
 * the number of keys, and a file path. The file at the given path is expected to contain
 * the values corresponding to the keys.
 */
SettingsInitializer::SettingsInitializer(String keys[], int keyCount, String filePath) {
    parameterCount = keyCount;
    configFilePath = filePath;
    parameters = new KeyValue[parameterCount];
    for (int i = 0; i < parameterCount; i++) {
        parameters[i].key = keys[i];
        parameters[i].value = "";
    }
}

/**
 * @brief Destructor for the SettingsInitializer class.
 *
 * This destructor deletes the dynamically allocated memory for the parameters array,
 * which holds the key-value pairs.
 */
SettingsInitializer::~SettingsInitializer() {
    delete[] parameters;
}

/**
 * @brief Initialize the settings
 *
 * This function initializes the settings by performing the following steps:
 *  1. Initialize the SD card with the given chip select pin.
 *  2. Retry the initialization up to 10 times if it fails.
 *  3. If the SD card is not found, display an error message.
 *  4. Open the config file on the SD card.
 *  5. If the config file is not found, display a warning message.
 *  6. Read the values from the config file and update the parameters.
 *  7. Flush and close the config file.
 */
void SettingsInitializer::begin() {
    bool sdStarted = SD.begin(SD_CS_PIN);
    for(int i = 0; i < 10 && !sdStarted; i++) {
        Log::getInstance().error("SD card initialization failed!");
        delay(1000);
        sdStarted = SD.begin(SD_CS_PIN);
    }

    if(!sdStarted) {
        StatusManager::getInstance().error("SD-Card not found", Colors::Red);
    }

    File configFile = SD.open(configFilePath.c_str());
    if (!configFile) {
        StatusManager::getInstance().error("SD-Card config file not found", Colors::Yellow);
    }
    readValues();

    configFile.flush();
    configFile.close();
}

/**
 * @brief Reads values from a configuration file and updates the corresponding key-value pairs.
 *
 * This function reads the configuration file specified by `configFilePath` and updates the
 * `value` attribute of the key-value pairs in the `parameters` array. Each line in the
 * configuration file should be in the format "key=value". The function searches for a matching
 * key in the `parameters` array and assigns the value to the corresponding key-value pair.
 */
void SettingsInitializer::readValues() {
    File configFile = SD.open(configFilePath.c_str());
    configFile.seek(0);

    while (configFile.available()) {
        String line = configFile.readStringUntil('\n');
        line.trim();
        int separatorIndex = line.indexOf('=');

        if (separatorIndex != -1) {
            String key = line.substring(0, separatorIndex);
            String value = line.substring(separatorIndex + 1);
            value.trim();
            key.trim();
            for (int i = 0; i < parameterCount; i++) {
                if(key == parameters[i].key) {
                    parameters[i].value = value;
                    break;
                }
            }
        }
    }
}

/**
 * @brief SettingsInitializer class
 *
 * This class is responsible for initializing and retrieving values from settings.
 */
const char* SettingsInitializer::getValue(String key) {
    for (int i = 0; i < parameterCount; i++) {
        if (parameters[i].key == key) {
            return parameters[i].value.c_str();
        }
    }
    return String("").c_str();
}
