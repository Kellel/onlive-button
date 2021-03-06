#include <Adafruit_SH110X.h>
#include <HTTPClient.h>

#include "interface.h"

#define STACK_SIZE 200

void Interface::submit_duration()
{
    HTTPClient http;

    String host = HOST;
    uint16_t port = PORT;
    String url = "/?state=true&duration=";

    http.begin(host, port, url + String(time));

    int resp_code = http.PUT(NULL, 0);

    if (resp_code > 0) {
        Serial.print("Server Response: ");
        Serial.println(resp_code);
        String payload = http.getString();
        Serial.println(payload);
    } 
    else 
    {
        Serial.print("Error: ");
        Serial.println(http.errorToString(resp_code).c_str());
    }
}

Interface::Interface(Adafruit_SH1107 *display, WifiComponent * wifi_icon, PowerComponent * power_icon, Button * left_button, Button * submit_button, Button * right_button)
{
    d = display;
    time = 15;
    last_state_change = millis();
    left = left_button;
    right = right_button;
    submit = submit_button;
    wifi_component = wifi_icon;
    power_component = power_icon;
}

unsigned long Interface::last_input()
{
    return last_state_change;
}

void Interface::input()
{
    if (left->debounce())
    {
        last_state_change = millis();
        Dec();
    }

    if (right->debounce())
    {
        last_state_change = millis();
        Inc();
    }

    if (submit->debounce() && wifi_component->connected())
    {
        last_state_change = millis();
        submit_duration();
    }

    power_component->measure();
}

void Interface::Inc()
{
    unsigned int tmp = time + step;
    if (tmp > 60)
    {
        tmp = tmp - 60;
    }

    time = tmp;
}

void Interface::Dec()
{
    int tmp = time - step;
    if (tmp < 0)
    {
        tmp = tmp + 60;
    }

    time = tmp;
}

void Interface::draw()
{
    d->setRotation(2);

    wifi_component->draw(d);
    // totalDisplayWidth = Size of wifi icon + size of bat icon + (5 pad *2) + X (space left over)
    // 64 = 16 + 16 + 5 + 5 + x
    // x = 22
    // middle space + size of wifi icon + pad
    // 22 + 16 + 5
    // = 43
    power_component->draw(d);

    // After the bitmaps
    // 64 = ((24*2) + x
    // x = 16 (total extra space, so 8px per side)
    d->setCursor(8, 32);
    d->setTextSize(4);
    d->printf("%2d", time);

    // Minute text
    // size 2 is 12x16
    // 64 = (12*3) + x
    // x = 28
    // y = 32 + 32 + padding (idk 10?)
    d->setCursor(14, 74);
    d->setTextSize(2);
    d->print("min");

    // Draw arrow keys
    d->setTextSize(1);
    // 128 = 8 + 5 + y
    // y = 107
    // 64 = (6*7) + x
    // x=22
    d->setCursor(11, 115);
    d->print("-  O  +");

    //Serial.print("Time: ");
    //Serial.println(time);
}