#include "RGBLed.h"

/**
 * @class RGBLed
 * @brief Represents a RGB LED.
 */
// Constructor
RGBLed::RGBLed(int redPin, int greenPin, int bluePin): _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

/**
 * @brief Sets the full color of the RGB LED.
 *
 * This method sets the full color of the RGB LED by setting the individual color components (red, green, and blue) using the corresponding pins.
 *
 * @param color The Color object that contains the RGB values.
 * @see void _writeColor(int pin, int value)
 */
void RGBLed::setColor(const Color& color) {
    _writeColor(_redPin, color.red);
    _writeColor(_greenPin, color.green);
    _writeColor(_bluePin, color.blue);
}

/**
 * @brief Sets the red color of the RGB LED.
 *
 * This method sets the red color of the RGB LED by writing the specified value to the red pin.
 * The value must be in the range of 0 to 255, where 0 indicates no red color and 255 indicates maximum red color.
 *
 * @param red The intensity of the red color (0-255).
 */
void RGBLed::setRed(int red) const {
    _writeColor(_redPin, red);
}

/**
 * @brief Sets the green value of the RGB LED.
 *
 * This method sets the green value of the RGB LED by writing the provided value to its green pin.
 *
 * @param green The value to set for the green component (0-255).
 */
void RGBLed::setGreen(int green) const {
    _writeColor(_greenPin, green);
}

/**
 * @brief Sets the blue color of the RGB LED.
 *
 * This method allows you to set only the blue color of the RGB LED.
 * The intensity of the blue color is specified by the 'blue' parameter.
 *
 * @param blue The intensity of the blue color (0-255).
 */
void RGBLed::setBlue(int blue) const {
    _writeColor(_bluePin, blue);
}

/**
 * @brief Private method to write to a specific pin.
 *
 * This method uses the `analogWrite` function to write a value to the specified pin.
 *
 * @param pin The pin number to write to.
 * @param value The analog value to write (0-255).
 *
 * @see RGBLed::setColor, RGBLed::setRed, RGBLed::setGreen, RGBLed::setBlue, RGBLed::off
 */
void RGBLed::_writeColor(int pin, int value) {
    analogWrite(pin, value);
}

/**
 * @brief Turns off the RGB LED by setting the color to Off.
 *
 * This method sets the color of the RGB LED to Off, which is equivalent to turning it off.
 * It internally calls the setColor method with the Off color, which sets all the color pins to 0.
 *
 * @see setColor()
 */
void RGBLed::off() {
    setColor(Colors::Off);
}
