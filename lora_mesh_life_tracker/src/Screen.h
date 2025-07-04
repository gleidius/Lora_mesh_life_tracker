#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include "variables.h"
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

extern Adafruit_SSD1306 my_display;

class Screen
{

private:
    HardwareSerial &mTerminal_UART;

public:
    void begin(void);

    void display();

    void fillRect(int x_pos, int y_pos); // закрашиваем прямоугольник

    void drawBitmap(int x, int y, const uint8_t bitmap[], int w, int h, uint16_t color); // отображаем рисунок по битмапу

    Display_coordinates cord;

    Screen(HardwareSerial &Serial1);

    void draw_in_coordinates(int x_pos, int y_pos, String text); // функция отрисовки по позиции, закрашивая строку

    Display_coordinates draw_menu(String module_address); // отрисовываем меню
};