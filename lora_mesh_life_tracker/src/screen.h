#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include "variables.h"
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

extern Adafruit_SSD1306 display;

class screen
{

private:
    HardwareSerial &MySerial1;

public:
    void begin(void);

    Display_coordinates cord;

    screen(HardwareSerial &Serial1);

    void draw_pos(int x_pos, int y_pos, String text); // функция отрисовки по позиции, закрашивая строку

    Display_coordinates init_menu(String Module_ADDR); // отрисовываем меню
};