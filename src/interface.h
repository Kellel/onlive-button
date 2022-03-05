#pragma once

#include <Adafruit_SH110X.h>
#include "button.h"

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


const char* ssid = "WIFI1.0";
const char* password = "jarjarbinks";

class Interface{
  public:
    Interface(Adafruit_SH1107 *d, unsigned int left_button, unsigned int submit_button, unsigned int right_button);
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

};