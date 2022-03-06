#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>

#include "interface.h"

const unsigned int D_H = 64;
const unsigned int D_W = 128;

Adafruit_SH1107 display = Adafruit_SH1107(D_H, D_W, &Wire);

#define BUTTON_A 15
#define BUTTON_B 32
#define BUTTON_C 14

Button button_left = Button(BUTTON_A);
Button button_submit = Button(BUTTON_B);
Button button_right = Button(BUTTON_C);
WifiComponent wifi_icon = WifiComponent(5, 0);
PowerComponent power_icon = PowerComponent(43, 0, A13);
Interface c = Interface(&display, &wifi_icon, &power_icon, &button_left, &button_submit, &button_right);

void setup()
{
  Serial.begin(115200);

  Serial.println("Starting button...");
  delay(250);                // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  wifi_icon.reconnect();

  display.setTextColor(SH110X_WHITE); 
  display.clearDisplay();
  display.display();
}

void loop()
{

  c.input();

  delay(10);
  yield();

  display.clearDisplay();
  c.draw();
  display.display();
}
