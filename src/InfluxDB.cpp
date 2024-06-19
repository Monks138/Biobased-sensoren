#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include "InfluxDB.h"
#include "Log.h"
#include <Adafruit_SleepyDog.h>

/**<
 * @brief InfluxDB constructor
 *
 * This constructor initializes the InfluxDB object with the given parameters.
 *
 * @param influxdb_url The URL of the InfluxDB instance
 * @param influxdb_org The organization within InfluxDB
 * @param influxdb_bucket The bucket within the organization
 * @param influxdb_token The access token for authentication
 */
InfluxDB::InfluxDB(char* influxdb_url, char* influxdb_org, char* influxdb_bucket, char* influxdb_token) {
    this->influxdb_url = influxdb_url;
    this->influxdb_org = influxdb_org;
    this->influxdb_bucket = influxdb_bucket;
    this->influxdb_token = influxdb_token;
}

/**
 * @brief Destructor for the InfluxDB class
 *
 * This destructor frees the memory allocated for the InfluxDB URL, organization, bucket, and token.
 * It should be called when the InfluxDB object is no longer needed.
 */
InfluxDB::~InfluxDB() {
    delete this->influxdb_url;
    delete this->influxdb_org;
    delete this->influxdb_bucket;
    delete this->influxdb_token;
}

/**
 * @brief Writes a single data point to an InfluxDB database using HTTP POST request.
 *
 * This function takes a single data point object in the form of a `Point` and sends it to the InfluxDB database using an
 * HTTP POST request. The data is formatted as a line protocol string before sending.
 *
 * @param point The `Point` object representing the data point to be written.
 * @param client The `HttpClient` object used to send the HTTP request.
 *
 * @return The HTTP status code received from the server, or -1 if the device is not connected to the network.
 */
int InfluxDB::writePoint(Point point, HttpClient& client) {
    arduino::String data = point.toLineProtocol();

    String url = "/write?db=" + String(this->influxdb_bucket);
    Log::getInstance().info("Sending request");

    client.beginRequest();
    client.setTimeout(1500);
    client.post(url);
    client.sendHeader("Authorization", String("Token ") + String(this->influxdb_token));
    client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
    client.sendHeader("Content-Length", data.length());
    client.beginBody();
    client.print(data);
    client.endRequest();

    Watchdog.reset();

    Log::getInstance().info("Request send");

    if(WiFi.status() == WL_CONNECTED) {
        int statusCode = client.responseStatusCode();
        String response = client.responseBody();

        Log::getInstance().info("Data received");
        Log::getInstance().info("Response: " + response);

        return statusCode;
    } else {
        return -1;
    }  
}

/**
 * @brief Writes an array of points to InfluxDB.
 *
 * This function takes an array of points along with the size of the array and an HttpClient object.
 * It converts each point to its line protocol representation and concatenates them with newline separators.
 * Then it sends a POST request to the InfluxDB server with the line protocol data in the request body.
 * The function also sets the necessary request headers such as authorization and content type.
 * Lastly, it waits for the response from the server and returns the HTTP status code.
 *
 * @param points An array of Point objects to write to InfluxDB.
 * @param size The size of the points array.
 * @param client The HttpClient object used to send requests to the InfluxDB server.
 * @return int The HTTP status code returned by the server.
 */
int InfluxDB::writePoints(Point* points, short size, HttpClient& client) {
    arduino::String data;
    for (int i = 0; i < size; i++) {
        data += points[i].toLineProtocol();
        if (i != sizeof(points) - 1) {
            data += "\n";
        }
    }

    String url = "/write?db=" + String(this->influxdb_bucket);
    Log::getInstance().info("Sending request");

    client.beginRequest();
    client.setTimeout(1500);
    client.post(url);
    client.sendHeader("Authorization", String("Token ") + String(this->influxdb_token));
    client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
    client.sendHeader("Content-Length", data.length());
    client.beginBody();
    client.print(data);
    client.endRequest();

    Watchdog.reset();

    Log::getInstance().info("Request send");

    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Log::getInstance().info("Data received");
    Log::getInstance().info("Response: " + response);

    return statusCode;
}

