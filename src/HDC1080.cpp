#include "HDC1080.h"
#include "Log.h"



HDC1080::HDC1080() : Sensor(), hdc()
{
}

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

float HDC1080::measure()
{
	return hdc.readTemperature();
}

float HDC1080::humidity()
{
    return hdc.readHumidity();
}

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