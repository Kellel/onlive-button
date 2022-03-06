#pragma once

#include <Adafruit_SH110X.h>
#include "button.h"
#include "wifi_icon.h"
#include "power_icon.h"

/*
 * Main interface It should look something like the following
 *  the display is 64x128 and we are going to run vertically
 *  at the bottom there are buttons that go from L->R rst, A,B,C
 * 
 *     ----------------
 *    | )))        === |
 *    |  ___     ___   |
 *    | /   \   /   \  |
 *    | |   |   |   |  |
 *    | |   |   |   |  |
 *    | \___/   \___/  |
 *    |                |
 *    | |\/|  |  |\  | |
 *    | |  |  |  | \ | |
 *    | |  |  |  |  \| |
 *    |                |
 *    |                |
 *    |  <-    o   ->  |
 *     ----------------
 * []
 *       []   []   []
 */



class Interface{
  public:
    Interface(Adafruit_SH1107 *d, WifiComponent * wifi_icon, PowerComponent * power_icon, Button * left_button, Button * submit_button, Button * right_button);
    void input();
    void Inc();
    void Dec();
    void draw();
  private:
    Adafruit_SH1107* d;
    Button * left;
    Button * submit;
    Button * right;
    unsigned int time = 0;
    unsigned int step = 15;
    WifiComponent * wifi_component;
    PowerComponent * power_component;
};