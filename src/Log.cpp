#include "Log.h"

Log::Log() {
    this->ntpClient = new NTPClient(*udpSocket, "pool.ntp.org", utcOffsetSummer);
    this->ntpClient->begin();
}

Log::~Log() {
    delete ntpClient;
    delete udpSocket;
}

Log& Log::getInstance() {
    static Log instance;
    return instance;
}

void Log::info(const arduino::String &message) {
    char* level = "INFO";
    this->serialPrintTime();
    Serial.print(level);
    Serial.print(": ");
    writeToCard(level, message);
}

void Log::error(const arduino::String &message) {
    char* level = "ERROR";
    this->serialPrintTime();
    Serial.print(level);
    Serial.print(": ");
    Serial.println(message);
    writeToCard(level, message);
}

void Log::writeToCard(const arduino::String &level, const arduino::String &message) {
    File file = SD.open(fileName, FILE_WRITE);
    file.write("[");
    file.write(ntpClient->getFormattedTime().c_str());
    file.write("] ");
    file.write(level.c_str());
    file.write(": ");
    file.write(message.c_str());
    file.write("\n");
    file.flush();
    file.close();
}

void Log::serialPrintTime() {
    ntpClient->update();
    Serial.print("[");
    Serial.print(ntpClient->getFormattedTime());
    Serial.print("] ");
}
