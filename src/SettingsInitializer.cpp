//
// Created by Merijn on 6/4/2024.
//
#include "SettingsInitializer.h"
#include <SPI.h>
#include <SD.h>

#define SD_CS_PIN 10  // Chip select pin for SD card module

SettingsInitializer::SettingsInitializer(String keys[], int keyCount, String filePath) {
    parameterCount = keyCount;
    configFilePath = filePath;
    parameters = new KeyValue[parameterCount];
    for (int i = 0; i < parameterCount; i++) {
        parameters[i].key = keys[i];
        parameters[i].value = "";
    }
}

SettingsInitializer::~SettingsInitializer() {
    delete[] parameters;
}

bool SettingsInitializer::begin() {
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD card initialization failed!");
        return false;
    }
    File configFile = SD.open(configFilePath.c_str());
    if (!configFile) {
        Serial.println("Failed to open config file!");
        return false;
    }
    for (int i = 0; i < parameterCount; i++) {
        parameters[i].value = readValueFromConfig(parameters[i].key);
    }
    configFile.close();
    return true;
}

String SettingsInitializer::readValueFromConfig(String key) {
    File configFile = SD.open(configFilePath.c_str());
    configFile.seek(0);
    while (configFile.available()) {
        String line = configFile.readStringUntil('\n');
        line.trim();
        if (line.startsWith(key)) {
            int separatorIndex = line.indexOf('=');
            if (separatorIndex != -1) {
                String value = line.substring(separatorIndex + 1);
                value.trim();
                return value;
            }
        }
    }
    return String("");
}

String SettingsInitializer::getValue(String key) {
    for (int i = 0; i < parameterCount; i++) {
        if (parameters[i].key == key) {
            return parameters[i].value;
        }
    }
    return String("");
}
