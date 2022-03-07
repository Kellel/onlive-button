#pragma once

#include <WiFi.h>
#include <Adafruit_SH110X.h>

#include "config.h"

#define ANIMATION_DELAY 500

class WifiComponent
{
public:
    WifiComponent(unsigned int x, unsigned int y);
    void reconnect();
    void disconnect();
    void draw(Adafruit_SH1107 * d);
    bool connected();

private:
    unsigned long lastDisplay;  
    wl_status_t lastStatus;
    unsigned int step;
    unsigned int x;
    unsigned int y;
};