//
// Created by Merij on 6/5/2024.
//

#ifndef BIOBASED_SENSOREN_STATUSMANAGER_H
#define BIOBASED_SENSOREN_STATUSMANAGER_H

#include "RGBLed.h"
#include "Color.h"



class StatusManager {
public:
    static StatusManager& getInstance();

    StatusManager(const StatusManager&) = delete;
    StatusManager& operator=(const StatusManager&) = delete;

    void setStatus(const Color& color);

    void error(String error, const Color& color, int errorCode = 5);

    void update();
    void setCurrentTime();

private:
    // Private constructor
    StatusManager();

    RGBLed _rgbLed;
    bool successBooted = false;
    int statusGreenTime = -1;
};

#endif //BIOBASED_SENSOREN_STATUSMANAGER_H
