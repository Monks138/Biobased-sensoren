#include "Log.h"

Log::Log() {
    this->ntpClient = new NTPClient(*udpSocket, "pool.ntp.org", utcOffsetSummer);
}

void Log::beginConnection() {
    //this->ntpClient->begin();
}

Log::~Log() {
    delete ntpClient;
    delete udpSocket;
}

Log& Log::getInstance() {
    static Log instance;
    return instance;
}

void Log::setDebugMode(bool mode) {
    this->debugMode = mode;
}

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

void Log::serialPrintTime() {
    if(WiFi.status() == WL_CONNECTED) {
        ntpClient->update();
        Serial.print("[");
        Serial.print(ntpClient->getFormattedTime());
        Serial.print("] ");
    }
}
