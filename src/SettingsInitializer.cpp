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

    configFile.close();
    configFile.flush();
}

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

const char* SettingsInitializer::getValue(String key) {
    for (int i = 0; i < parameterCount; i++) {
        if (parameters[i].key == key) {
            return parameters[i].value.c_str();
        }
    }
    return String("").c_str();
}
