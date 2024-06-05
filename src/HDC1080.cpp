#include "HDC1080.h"



HDC1080::HDC1080(uint8_t i2cAddress) : Sensor(i2cAddress), hdc()
{
}

void HDC1080::begin()
{
    Wire.begin();
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
	char format[12];
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
