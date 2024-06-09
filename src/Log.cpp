#include "Log.h"

Log::Log(bool writeToCard) : writeDataToCard(writeToCard) {
    this->ntpClient = new NTPClient(*udpSocket, "pool.ntp.org", utcOffsetWinter);
    this->ntpClient->begin();
}

Log::~Log() {
}

void Log::info(char* message) {
    char* level = "INFO";
    Serial.print(level);
    Serial.print(": ");
    writeToCard(level, message);
}

void Log::error(char* message) {
    char* level = "ERROR";
    Serial.print(level);
    Serial.print(": ");
    Serial.println(message);
    writeToCard(level, message);
}

void Log::writeToCard(char* level, char* message) {
    ntpClient->update();
    if(this->writeDataToCard) {
        File file = SD.open(fileName, FILE_WRITE);
        file.write("[");
        file.write(ntpClient->getFormattedTime().c_str());
        file.write("] ");
        file.write(" ");
        file.write(level);
        file.write(": ");
        file.write(message);
        file.write("\n");
        file.flush();
        file.close();
    }
}