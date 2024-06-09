#pragma once
#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include "Point.h"

class InfluxDB {
    public:
        InfluxDB(char* influxdb_url, char* influxdb_org, char* influxdb_bucket, char* influxdb_token);
        ~InfluxDB();
        void writePoint(Point point, HttpClient& client);
        void writePoints(Point* points, short size, HttpClient& client);
    private:
        char* influxdb_url;
        char* influxdb_org;
        char* influxdb_bucket;
        char* influxdb_token;
};

