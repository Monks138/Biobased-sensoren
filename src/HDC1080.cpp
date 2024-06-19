#include "HDC1080.h"
#include "Log.h"



/**
 * @class HDC1080
 * @brief A class representing the HDC1080 sensor.
 *
 * This class provides functions to interact with the HDC1080 sensor. It inherits from the Sensor class.
 */
HDC1080::HDC1080() : Sensor(), hdc()
{
}

/**
 * @brief Initializes the HDC1080 sensor and checks if it is functioning properly
 *
 * This function initializes the HDC1080 sensor by calling the `begin` function of the `ClosedCube_HDC1080` object.
 * It then checks if the sensor is working by repeatedly reading its device ID until a valid value is obtained.
 * If the device ID is 0xFF, it prints an error message indicating that the sensor is not working or not connected.
 *
 * After that, it reads the manufacturer ID, device ID, and serial number of the sensor, and prints them to the serial monitor.
 * The serial number is formatted as "XX-XXXX-XXXX" and printed as a hexadecimal value.
 *
 * @note This function assumes that the `Wire` library has already been initialized.
 *
 * @see ClosedCube_HDC1080::begin()
 * @see ClosedCube_HDC1080::readDeviceId()
 * @see ClosedCube_HDC1080::readManufacturerId()
 * @see ClosedCube_HDC1080::readSerialNumber()
 */
void HDC1080::begin()
{
//    Wire.begin();
    hdc.begin(0x40);

    while(hdc.readDeviceId() == 0xFF)
    {
        Serial.println("HDC Sensor doesn't work or is not connected");
        delay(1000);
    }

    Serial.print("Manufacturer ID=0x");
	Serial.println(hdc.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
	Serial.print("Device ID=0x");
	Serial.println(hdc.readDeviceId(), HEX); // 0x1050 ID of the device
	Serial.print("Device Serial Number=");
	HDC1080_SerialNumber sernum = hdc.readSerialNumber();
	char format[100];
	sprintf(format, "%02X-%04X-%04X", sernum.serialFirst, sernum.serialMid, sernum.serialLast);
	Serial.println(format);
}

/**
 * @class HDC1080
 * @brief Class representing the HDC1080 sensor.
 *
 * This class provides methods to measure temperature and humidity using the HDC1080 sensor.
 */
float HDC1080::measure()
{
	return hdc.readTemperature();
}

/**
 * @brief Returns the humidity measured by the HDC1080 sensor.
 *
 * This function reads the humidity value from the HDC1080 sensor
 * and returns it as a floating-point number.
 *
 * @return The humidity value as a floating-point number.
 */
float HDC1080::humidity()
{
    return hdc.readHumidity();
}

/**
 * @brief Retrieves measurement points from the HDC1080 sensor.
 *
 * This function measures the temperature and humidity using the HDC1080 sensor and constructs SensorPoint object with the measurement points.
 *
 * @param room The name of the room where the measurement is taken.
 * @param macAddress The MAC address of the sensor.
 * @return A SensorPoint object containing the measurement points.
 */
SensorPoint HDC1080::getMeasurementPoints(const char* room, char* macAddress) {
    SensorPoint sensorPoint = SensorPoint();
    short size = 2;

    sensorPoint.size = size;
    sensorPoint.points = new Point[size];


    Log::getInstance().info("Retrieving Temp");
    float temp = this->measure();
    Log::getInstance().info("Got Temp, Retrieving Humidty");
    float humidity = this->humidity();
    Log::getInstance().info("Got Humidity");

    sensorPoint.points[0] = Point().measurement("temperature_sensor")
        .addTag("room", room)
        .addTag("sensor_id", macAddress)
        .addTag("unit", "degrees_celsius")
        .addField("temperature", temp);
    sensorPoint.points[1] = Point().measurement("humidity_sensor")
        .addTag("room", room)
        .addTag("sensor_id", macAddress)
        .addTag("unit", "percentage")
        .addField("humidity", humidity);
    Serial.println("Data measured!");


    return sensorPoint;
}