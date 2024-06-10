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

int InfluxDB::writePoint(Point point, HttpClient& client) {
    arduino::String data = point.toLineProtocol();

    String url = "/write?db=" + String(this->influxdb_bucket);

    client.beginRequest();
    client.setTimeout(5000);
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

    return statusCode;
}

int InfluxDB::writePoints(Point* points, short size, HttpClient& client) {
    arduino::String data;
    for (int i = 0; i < size; i++) {
        data += points[i].toLineProtocol();
        if (i != sizeof(points) - 1) {
            data += "\n";
        }
    }

    String url = "/write?db=" + String(this->influxdb_bucket);

    client.beginRequest();
    client.setTimeout(5000);
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

    return statusCode;
}

