#include <Adafruit_SH110X.h>

#include "wifi_icon.h"

#define net_wifi_1_width 16
#define net_wifi_1_height 16
static unsigned char net_wifi_1_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
    0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00};

#define net_wifi_2_width 16
#define net_wifi_2_height 16
static unsigned char net_wifi_2_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
    0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00};

#define net_wifi_3_width 16
#define net_wifi_3_height 16
static unsigned char net_wifi_3_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x10, 0x08,
    0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
    0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00};

#define net_wifi_4_width 16
#define net_wifi_4_height 16
static unsigned char net_wifi_4_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x08, 0x10, 0xe4, 0x27, 0x10, 0x08,
    0xc0, 0x03, 0x20, 0x04, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
    0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00};

#define net_wifi_none_width 16
#define net_wifi_none_height 16
static unsigned char net_wifi_none_bits[] = {
    0x00, 0x3f, 0x80, 0xfd, 0x80, 0xff, 0x80, 0xff, 0x80, 0xff, 0xc0, 0x0f,
    0xe1, 0x7f, 0xf3, 0x07, 0xff, 0x07, 0xff, 0x1f, 0xfe, 0x17, 0xfc, 0x03,
    0xf8, 0x01, 0x20, 0x01, 0x20, 0x01, 0x60, 0x03};

void WifiComponent::reconnect()
{
    String ssid = SSID;
    String password = PASSWORD;

    WiFi.disconnect();
    Serial.print("Connecting to wifi: ");
    Serial.println(ssid);

    WiFi.begin(ssid.c_str(), password.c_str());
}

void WifiComponent::disconnect()
{
    WiFi.disconnect();
}

WifiComponent::WifiComponent(unsigned int pos_x, unsigned int pos_y)
{
    x = pos_x;
    y = pos_y;
    step = 0;
    lastStatus = WL_DISCONNECTED;
    lastDisplay = millis();
}

void WifiComponent::draw(Adafruit_SH1107 *d)
{
    wl_status_t status = WiFi.status();
    switch (status)
    {
    case WL_CONNECTED:
        if (lastStatus != WL_CONNECTED)
        {
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
        }
        d->drawXBitmap(x, y, net_wifi_4_bits, net_wifi_4_width, net_wifi_4_height, 1);
        break;
    case WL_NO_SHIELD:
        if (lastStatus != WL_NO_SHIELD)
        {
            Serial.println("WiFi connection failed");
        }
        d->drawXBitmap(x, y, net_wifi_none_bits, net_wifi_none_width, net_wifi_none_height, 1);
        break;
    case WL_DISCONNECTED:
    case WL_SCAN_COMPLETED:
    case WL_IDLE_STATUS:
    {
        if (lastStatus != WL_DISCONNECTED)
        {
            Serial.println("WiFI Connecting");
        }
        switch (step)
        {
        case 0:
            d->drawXBitmap(x, y, net_wifi_1_bits, net_wifi_1_width, net_wifi_1_height, 1);
            break;
        case 1:
            d->drawXBitmap(x, y, net_wifi_2_bits, net_wifi_2_width, net_wifi_2_height, 1);
            break;
        case 2:
            d->drawXBitmap(x, y, net_wifi_3_bits, net_wifi_3_width, net_wifi_3_height, 1);
            break;
        case 3:
            d->drawXBitmap(x, y, net_wifi_4_bits, net_wifi_4_width, net_wifi_4_height, 1);
            break;
        }

        unsigned long current_time = millis();
        if (lastDisplay + ANIMATION_DELAY < current_time)
        {
            lastDisplay = current_time;
            step++;
            if (step > 3)
            {
                step = 0;
            }
        }
        break;
    }
    default:
        if (status != lastStatus)
        {
            Serial.print("WiFI error: ");
            Serial.println(status);
        }
        d->drawXBitmap(x, y, net_wifi_none_bits, net_wifi_none_width, net_wifi_none_height, 1);
        break;
    }

    lastStatus = status;
}

bool WifiComponent::connected() {
    return lastStatus == WL_CONNECTED ? true : false;
}