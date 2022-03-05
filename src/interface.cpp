#include "interface.h"

#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include "button.h"

// Shamelessly stolen from https://github.com/Feh/configs/blob/master/.awesome-icons/power-bat2.xbm
#define net_wifi4_width 16
#define net_wifi4_height 16
static unsigned char net_wifi4_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x08, 0x10, 0xe4, 0x27, 0x10, 0x08,
    0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
    0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00};

#define power_bat2_width 16
#define power_bat2_height 16
static unsigned char power_bat2_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x1f, 0x7e, 0x18,
    0x7e, 0x78, 0x7e, 0x78, 0x7e, 0x78, 0x7e, 0x78, 0x7e, 0x18, 0xfe, 0x1f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

Interface::Interface(Adafruit_SH1107 *display, unsigned int left_button, unsigned int submit_button, unsigned int right_button)
{
    d = display;
    time = 0;
    left = &Button(left_button);
    right = &Button(right_button);
    submit = &Button(submit_button);

    int ret = wifi_init();
    switch (ret)
    {
        // text display tests
        d->setTextSize(1);
        d->setTextColor(SH110X_WHITE);
        d->setCursor(0, 0);

        switch (wifi_init())
        {
        case 0:
            d->print("Connected to: ");
            d->println(ssid);
            d->print("IP: ");
            d->println(WiFi.localIP());
            break;
        case 1:
            d->print("Failed to connect to: ");
            d->println(ssid);
            d->print("reason: ");
            d->println("TIMEOUT");
            break;
        case -1:
            d->print("Failed to connect to: ");
            d->println(ssid);
            d->print("reason: ");
            d->println("FAIL");
            break;
        }

        d->display(); // actually display all of the above
    }
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
    Serial.print("Time: ");
    Serial.println(time);
    d->setRotation(2);
    d->drawXBitmap(5, 0, net_wifi4_bits, net_wifi4_width, net_wifi4_height, 1);
    // totalDisplayWidth = Size of wifi icon + size of bat icon + (5 pad *2) + X (space left over)
    // 64 = 16 + 16 + 5 + 5 + x
    // x = 22
    // middle space + size of wifi icon + pad
    // 22 + 16 + 5
    // = 43
    d->drawXBitmap(43, 0, power_bat2_bits, power_bat2_width, power_bat2_height, 1);

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
}

int wifi_init()
{
    Serial.print("Connecting to wifi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    const int wifi_timeout = 100;

    for (int i = 0; i < wifi_timeout; i++)
    {
        switch (WiFi.status())
        {
        case WL_CONNECTED:
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            return 0;

        // case WL_CONNECT_FAILED:
        //   Serial.println("WIFI connection failed");
        //   return -1;
        default:
            delay(500);
            Serial.print(".");
        }
    }

    return 1;
}