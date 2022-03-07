#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_bt.h>
#include <driver/rtc_io.h>

#include "interface.h"
#include "config.h"

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

uint64_t gen_mask(uint a, uint b, uint c)
{
  return 1 << a | 1 << b | 1 << c;
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void setup()
{
  Serial.begin(115200);

  Serial.println("Starting button...");
  delay(250);                // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  print_wakeup_reason();

  wifi_icon.reconnect();

  display.setTextColor(SH110X_WHITE);
  display.clearDisplay();
  display.display();

  // Setup wakeup stuff
  uint64_t mask = gen_mask(BUTTON_A, BUTTON_B, BUTTON_C);
  esp_sleep_enable_ext1_wakeup(mask, ESP_EXT1_WAKEUP_ANY_HIGH);

  unsigned long time = millis();
  unsigned long sleep_time = SLEEP_TIME * 1000;

  for (;;)
  {
    if ((c.last_input() + sleep_time) < time)
    {
      // Go to sleep since we haven't seen anything in a bit
      Serial.println("SLEEPING NOW");
      Serial.print("Now: ");
      Serial.print(time);
      Serial.print(" last: ");
      Serial.print(c.last_input());
      Serial.print(" sleep_time:" );
      Serial.println(sleep_time);
      //rtc_gpio_pullup_en((gpio_num_t) BUTTON_A);
      //rtc_gpio_pullup_en((gpio_num_t) BUTTON_B);
      //rtc_gpio_pullup_en((gpio_num_t) BUTTON_C);
      wifi_icon.disconnect();
      esp_bt_controller_disable();
      esp_wifi_stop();
      display.oled_command(SH110X_DISPLAYOFF);
      esp_deep_sleep_start();
    }

    c.input();

    delay(10);
    yield();

    display.clearDisplay();
    c.draw();
    display.display();
    time = millis();
  }
}

void loop() {}