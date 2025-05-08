#include "menu.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Menu menu;

void Menu::setup(Button *buttonOK, Button *buttonUP, Button *buttonDown)
{
    this->buttonOK = buttonOK;
    this->buttonUP = buttonUP;
    this->buttonDown = buttonDown;

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        DEBUGLN("Display SSD1306 allocation failed");
        for (;;)
            ;
    }

    display.display();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);
}

// void Menu::drawPos(int x_pos, int y_pos, char *text) // функция отрисовки по позиции, закрашивая строку
// {
//     display.setCursor(x_pos, y_pos);
//     display.fillRect(x_pos, y_pos, 128, 8, SSD1306_BLACK);
//     display.setTextColor(BLACK, WHITE);
//     display.print(text);
//     display.display();
// }

// void Menu::drawPos(int x_pos, int y_pos, int &text)
// {
//     display.setCursor(x_pos, y_pos);
//     display.fillRect(x_pos, y_pos, 128, 8, SSD1306_BLACK);
//     display.setTextColor(BLACK, WHITE);
//     display.print(text);
//     display.display();
// }