#include "Point.h"

Point::Point() {
    
}

Point::~Point() {
    
}

Point& Point::addTag(const char* tag, const char* value) {
    if (tags_.length() > 0) {
        tags_ += ",";
    }
    tags_ += arduino::String(tag) + "=" + arduino::String(value);
    return *this;
}

Point& Point::addField(const char* field, const char* value) {
    if (fields_.length() > 0) {
        fields_ += ",";
    }
    fields_ += arduino::String(field) + "=" + arduino::String(value);
    return *this;
}

Point& Point::addField(const char* field, double value) {
    if (fields_.length() > 0) {
        fields_ += ",";
    }
    fields_ += arduino::String(field) + "=" + arduino::String(value);
    return *this;
}

Point& Point::measurement(const char* measurement) {
    measurement_ = arduino::String(measurement);
    return *this;
}

arduino::String Point::toLineProtocol() {
    arduino::String data = arduino::String(measurement_) + "," + arduino::String(tags_) + " " + arduino::String(fields_);
    return data;
}