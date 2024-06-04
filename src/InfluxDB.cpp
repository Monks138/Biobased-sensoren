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

    float temperature = 24.5;
    float humidity = 60.2;

    String exampleData = "environment,device=arduino_nano temperature=" + String(temperature) + ",humidity=" + String(humidity);
    Serial.print("Expected data like: ");
    Serial.println(exampleData);


    String url = "/write?db=" + String(this->influxdb_bucket);

    client.beginRequest();
    client.post(url);
    client.sendHeader("Authorization", String("Token ") + String(this->influxdb_token));
    client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
    client.sendHeader("Content-Length", data.length());
    client.beginBody();
    client.print(data);
    client.endRequest();

    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
}

