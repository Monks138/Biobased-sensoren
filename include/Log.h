#pragma once

#include <Arduino.h>
#include <SD.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define fileName "log.txt"
#define SD_CS_PIN 4 

const long utcOffsetWinter = 3600; // Offset from UTC in seconds (3600 seconds = 1h) -- UTC+1 (Central European Winter Time)
const long utcOffsetSummer = 7200; // Offset from UTC in seconds (7200 seconds = 2h) -- UTC+2 (Central European Summer Time)

class Log {
    public:
        Log(bool writeToCard = false);
        ~Log();
        void info(char* message);
        void error(char* message);
    private:
        void writeToCard(char* level, char* message);
        bool writeDataToCard;
        WiFiUDP* udpSocket;
        NTPClient* ntpClient;
};