#pragma once

#include <Adafruit_SH110X.h>



typedef enum
{
    BATT_NONE = -1,
    BATT_LOW  = 0,
    BATT_MID  = 1,
    BATT_FULL = 2
} batt_status_t;

class PowerComponent
{
public:
    PowerComponent(unsigned int x, unsigned int y, unsigned int battery_pin);
    void measure();
    void draw(Adafruit_SH1107 *d);

private:
    unsigned int x;
    unsigned int y;
    batt_status_t status;
    batt_status_t lastStatus;
    unsigned int pin;
};