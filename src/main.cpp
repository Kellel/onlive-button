#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>

#include "interface.h"
#include "button.h"

const unsigned int D_H = 64;
const unsigned int D_W = 128;

Adafruit_SH1107 display = Adafruit_SH1107(D_H, D_W, &Wire);

#define BUTTON_A 15
#define BUTTON_B 32
#define BUTTON_C 14

const char* ssid = "xxxxx";
const char* password = "xxxxx";

Clock c = Clock(&display);
Button button_a;
Button button_c;


int wifi_init() {
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  const int wifi_timeout = 100;

  for (int i = 0; i < wifi_timeout; i++) {
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

void setup() {
  Serial.begin(115200);

  Serial.println("Starting button...");
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  //display.display();
  //delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);

  switch (wifi_init())
  {
    case 0:
      display.print("Connected to: ");
      display.println(ssid);
      display.print("IP: ");
      display.println(WiFi.localIP());
      break;
    case 1:
      display.print("Failed to connect to: ");
      display.println(ssid);
      display.print("reason: ");
      display.println("TIMEOUT");
      break;
    case -1:
      display.print("Failed to connect to: ");
      display.println(ssid);
      display.print("reason: ");
      display.println("FAIL");
      break;
  }

  display.display(); // actually display all of the above
  delay(1000);
  display.clearDisplay();
  display.display();

  button_a.begin(BUTTON_A);
  button_c.begin(BUTTON_C);

}


void loop() {
                   
  if (button_a.debounce()) c.Inc();
  if (button_c.debounce()) c.Dec();

  delay(10);
  yield();

  display.clearDisplay();
  c.draw();
  display.display();
}
