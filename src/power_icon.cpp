#include "power_icon.h"

#define power_bat_mid_width 16
#define power_bat_mid_height 16
static unsigned char power_bat_mid_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x1f, 0x7e, 0x18,
   0x7e, 0x78, 0x7e, 0x78, 0x7e, 0x78, 0x7e, 0x78, 0x7e, 0x18, 0xfe, 0x1f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define power_bat_full_width 16
#define power_bat_full_height 16
static unsigned char power_bat_full_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x1f, 0xfe, 0x1f,
    0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x7f, 0xfe, 0x1f, 0xfe, 0x1f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define power_bat_empty_width 16
#define power_bat_empty_height 16
static unsigned char power_bat_empty_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x1f, 0x02, 0x10,
    0x02, 0x70, 0x02, 0x70, 0x02, 0x70, 0x02, 0x70, 0x02, 0x10, 0xfe, 0x1f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define power_bat_none_width 16
#define power_bat_none_height 16
static unsigned char power_bat_none_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x06, 0x00, 0x7f,
   0x80, 0x07, 0xfc, 0x07, 0xfc, 0x07, 0x80, 0x07, 0x00, 0x7f, 0x00, 0x06,
   0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

PowerComponent::PowerComponent(unsigned int pos_x, unsigned int pos_y, unsigned int batt_pin)
{
    x = pos_x;
    y = pos_y;
    lastStatus = BATT_NONE;
    status = BATT_NONE;
    pin = batt_pin;
    pinMode(pin, INPUT_PULLDOWN);
}

void PowerComponent::measure()
{
    unsigned int value = analogRead(pin);

    // Analog read will give us a value from 1-4095
    // to represent a voltage between 0.0 and 5.0
    // so lets convert that to voltage

    float voltage = value * (3.3 / 4094.0) * 2;
    Serial.print("Bat reading: ");
    Serial.println(value);
    Serial.print("Bat voltage: ");
    Serial.println(voltage);

    if (voltage > 4.5)
    {
        status = BATT_NONE;
    }
    else if (voltage > 3.8 && voltage <= 4.5)
    {
        status = BATT_FULL;
    }
    else if (voltage > 3.5 && voltage <= 3.8)
    {
        status = BATT_MID;
    }
    else if (voltage <= 3.5)
    {
        status = BATT_LOW;
    }
}

void PowerComponent::draw(Adafruit_SH1107 *d)
{
    if (lastStatus != status)
    {
        Serial.print("Batt Voltage status: ");
        Serial.println(status);
    }

    switch (status)
    {
    case BATT_FULL:
        d->drawXBitmap(x, y, power_bat_full_bits, power_bat_full_width, power_bat_full_height, 1);
        break;
    case BATT_MID:
        d->drawXBitmap(x, y, power_bat_mid_bits, power_bat_mid_width, power_bat_mid_height, 1);
        break;
    case BATT_LOW:
        d->drawXBitmap(x, y, power_bat_empty_bits, power_bat_empty_width, power_bat_empty_height, 1);
        break;
    case BATT_NONE:
        d->drawXBitmap(x, y, power_bat_none_bits, power_bat_none_width, power_bat_none_height, 1);
        break;
    }
    lastStatus = status;
}