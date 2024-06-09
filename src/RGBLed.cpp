#include "RGBLed.h"

// Constructor
RGBLed::RGBLed(int redPin, int greenPin, int bluePin): _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

// Methode om de volledige kleur te stellen
void RGBLed::setColor(const Color& color) {
    _writeColor(_redPin, color.red);
    _writeColor(_greenPin, color.green);
    _writeColor(_bluePin, color.blue);
}

// Methode om alleen rood in te stellen
void RGBLed::setRed(int red) const {
    _writeColor(_redPin, red);
}

// Methode om alleen groen in te stellen
void RGBLed::setGreen(int green) const {
    _writeColor(_greenPin, green);
}

// Methode om alleen blauw in te stellen
void RGBLed::setBlue(int blue) const {
    _writeColor(_bluePin, blue);
}

// Privé methode om een specifieke pin te schrijven
void RGBLed::_writeColor(int pin, int value) {
    analogWrite(pin, value);
}

void RGBLed::off() {
    setColor(Colors::Off);
}
