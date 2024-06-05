#pragma once
#include <Arduino.h>

class Point {
    public:
        Point();
        ~Point();
        Point& addTag(const char* tag, const char* value);
        Point& addField(const char* field, const char* value);
        Point& addField(const char* field, double value);
        Point& measurement(const char* measurement);
        arduino::String toLineProtocol();
    private:
        arduino::String measurement_;
        arduino::String tags_;
        arduino::String fields_;
};