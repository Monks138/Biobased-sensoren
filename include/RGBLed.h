#ifndef BIOBASED_SENSOREN_RGBLED_H
#define BIOBASED_SENSOREN_RGBLED_H

#include <Arduino.h>

class RGBLed {
public:
    // Constructor
    RGBLed(int redPin, int greenPin, int bluePin);

    // Methoden om de kleuren in te stellen
    void setColor(int red, int green, int blue);
    void setRed(int red);
    void setGreen(int green);
    void setBlue(int blue);

private:
    int _redPin;
    int _greenPin;
    int _bluePin;

    void _writeColor(int pin, int value);
};

#endif
