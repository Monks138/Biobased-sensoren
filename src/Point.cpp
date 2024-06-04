#include "Point.h"

Point::Point() {
    
}

Point::~Point() {
    
}

Point& Point::addTag(const char* tag, const char* value) {
    if (tags_.length() > 0) {
        tags_ += ",";
    }
    tags_ += String(tag) + "=" + String(value);
    return *this;
}

Point& Point::addField(const char* field, const char* value) {
    if (fields_.length() > 0) {
        fields_ += ",";
    }
    fields_ += String(field) + "=" + String(value);
    return *this;
}

Point& Point::addField(const char* field, double value) {
    if (fields_.length() > 0) {
        fields_ += ",";
    }
    fields_ += String(field) + "=" + String(value);
    return *this;
}

Point& Point::measurement(const char* measurement) {
    measurement_ = String(measurement);
    return *this;
}

String Point::toLineProtocol() {
    return measurement_ + "," + tags_ + " " + fields_;
}