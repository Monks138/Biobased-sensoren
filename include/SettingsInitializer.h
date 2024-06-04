//
// Created by Merijn on 6/4/2024.
//
#ifndef BIOBASED_SENSOREN_SETTINGSINITIALIZER_H
#define BIOBASED_SENSOREN_SETTINGSINITIALIZER_H

#include <Arduino.h>

class SettingsInitializer {
private:
    struct KeyValue {
        String key;
        String value;
    };
    KeyValue* parameters;
    int parameterCount;
    String configFilePath;

    void readValues();

public:
    SettingsInitializer(String keys[],int keyCount, String filePath);
    ~SettingsInitializer();

    bool begin();
    String getValue(String key);
};

#endif

