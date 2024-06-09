#ifndef BIOBASED_SENSOREN_RGBLED_H
#define BIOBASED_SENSOREN_RGBLED_H

#include <Arduino.h>
#include "Color.h"

class RGBLed {
public:
    // Constructor
    RGBLed(int redPin, int greenPin, int bluePin);

    // Methoden om de kleuren in te stellen met Color struct
    void setColor(const Color& color);
    void setRed(int red) const;
    void setGreen(int green) const;
    void setBlue(int blue) const;
    void off();

private:
    int _redPin;
    int _greenPin;
    int _bluePin;

    static void _writeColor(int pin, int value);
};

#endif
