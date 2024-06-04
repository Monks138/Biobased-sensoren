#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include "InfluxDB.h"

InfluxDB::InfluxDB(char* influxdb_url, char* influxdb_org, char* influxdb_bucket, char* influxdb_token) {
    this->influxdb_url = influxdb_url;
    this->influxdb_org = influxdb_org;
    this->influxdb_bucket = influxdb_bucket;
    this->influxdb_token = influxdb_token;
}

InfluxDB::~InfluxDB() {
    delete this->influxdb_url;
    delete this->influxdb_org;
    delete this->influxdb_bucket;
    delete this->influxdb_token;
}

void InfluxDB::writePoint(Point point, HttpClient& client) {
    arduino::String data = point.toLineProtocol();

    Serial.println("Writing...");

    arduino::String url = arduino::String("/api/v2/write?org=") + this->influxdb_org + "&bucket=" + this->influxdb_bucket + "&precision=s";
    client.beginRequest();
    client.post(url);
    client.sendHeader("Authorization", String("Token ") + this->influxdb_token);
    client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
    client.sendHeader("Content-Length", data.length());
    client.beginBody();
    client.print(data);
    client.endRequest();

    Serial.println(client.responseStatusCode());
}

