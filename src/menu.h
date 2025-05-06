#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "configuration.h"

extern Adafruit_SSD1306 display;

// class Menu
// {
// public:
//     void setup();
//     void addTable();
//     void addField();
//     void drawPos(int x_pos, int y_pos, String text);
//     void menuHandler();

//     void nextTable();
//     void prevTable();
//     void leftField();
//     void rightField();

// private:
//     uint16_t currentTable = 0;
//     uint16_t currentField = 0;
// };

// extern Menu menu;
