#include "Point.h"

/**
 * @class Point
 * @brief The Point class represents a data point in a measurement.
 *
 * This class is used to construct and manipulate data points for sending to a
 * database or for other data processing purposes.
 */
Point::Point() {
    
}

/**
 * @brief Destructor for the Point class.
 *
 * This destructor is responsible for destroying objects of the Point class.
 * It is used to release any resources that were allocated within the object.
 *
 * @note The destructor does not take any parameters.
 *
 * @sa Point
 */
Point::~Point() {
    
}

/**
 * @brief Adds a tag to the point object.
 *
 * This function adds a tag to the existing tags_ string.
 * If the tags_ string is not empty, a comma is added before appending the new tag.
 * The tag is in the format "tag=value".
 *
 * @param tag The tag name.
 * @param value The tag value.
 * @return A reference to the updated Point object.
 */
Point& Point::addTag(const char* tag, const char* value) {
    if (tags_.length() > 0) {
        tags_ += ",";
    }
    tags_ += arduino::String(tag) + "=" + arduino::String(value);
    return *this;
}

/**
 * @brief Add a field to the Point object.
 *
 * This function adds a field to the Point object. The field is a key-value pair where the key is specified by the `field` parameter and the value is specified by the `value` parameter
*. The field is added to the `fields_` member of the Point object.
 *
 * @param field The key of the field to add.
 * @param value The value of the field to add.
 * @return A reference to the modified Point object.
 */
Point& Point::addField(const char* field, const char* value) {
    if (fields_.length() > 0) {
        fields_ += ",";
    }
    fields_ += arduino::String(field) + "=" + arduino::String(value);
    return *this;
}

/**
 * @class Point
 * @brief Represents a point in a data measurement system
 *
 * The Point class is used to represent a point in a data measurement system. It allows the addition of fields
 * and values to the point, which can later be converted to a line protocol string for communication.
 */
Point& Point::addField(const char* field, double value) {
    if (fields_.length() > 0) {
        fields_ += ",";
    }
    fields_ += arduino::String(field) + "=" + arduino::String(value);
    return *this;
}

/**
 * @brief Sets the measurement of the Point object.
 *
 * This function sets the measurement of the Point object. The measurement is stored as an arduino::String.
 *
 * @param measurement The measurement to set.
 * @return A reference to the Point object for method chaining.
 */
Point& Point::measurement(const char* measurement) {
    measurement_ = arduino::String(measurement);
    return *this;
}

/**
 * @brief Converts the Point object to a line protocol string.
 *
 * This function combines the measurement, tags, and fields of the Point object
 * into a comma-separated string representation, following the InfluxDB line protocol format.
 * The resulting string can be used to insert data into an InfluxDB database.
 *
 * @return The line protocol string representation of the Point object.
 */
arduino::String Point::toLineProtocol() {
    arduino::String data = arduino::String(measurement_) + "," + arduino::String(tags_) + " " + arduino::String(fields_);
    return data;
}