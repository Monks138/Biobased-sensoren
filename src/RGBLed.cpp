#include "RGBLED.h"

// Constructor
RGBLed::RGBLed(int redPin, int greenPin, int bluePin): _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

// Methode om alle kleuren in te stellen
void RGBLed::setColor(int red, int green, int blue) {
    _writeColor(_redPin, red);
    _writeColor(_greenPin, green);
    _writeColor(_bluePin, blue);
}

// Methode om alleen rood in te stellen
void RGBLed::setRed(int red) {
    _writeColor(_redPin, red);
}

// Methode om alleen groen in te stellen
void RGBLed::setGreen(int green) {
    _writeColor(_greenPin, green);
}

// Methode om alleen blauw in te stellen
void RGBLed::setBlue(int blue) {
    _writeColor(_bluePin, blue);
}

// Privé methode om een specifieke pin te schrijven
void RGBLed::_writeColor(int pin, int value) {
    analogWrite(pin, value);
}
