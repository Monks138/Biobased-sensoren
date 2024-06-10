#pragma once

#include <Arduino.h>
#include <SD.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiNINA.h>

#define fileName "log.txt"
#define SD_CS_PIN 4 

const long utcOffsetWinter = 3600; // Offset from UTC in seconds (3600 seconds = 1h) -- UTC+1 (Central European Winter Time)
const long utcOffsetSummer = 7200; // Offset from UTC in seconds (7200 seconds = 2h) -- UTC+2 (Central European Summer Time)

class Log {
    public:
        Log();
        ~Log();
        static Log& getInstance();
        void info(const arduino::String &message);
        void error(const arduino::String &message);
    private:
        void writeToCard(const arduino::String &level, const arduino::String &message);
        void serialPrintTime();
        WiFiUDP* udpSocket;
        NTPClient* ntpClient;
};