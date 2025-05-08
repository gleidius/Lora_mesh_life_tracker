#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "configuration.h"

#include "menuStructure.h"

extern Adafruit_SSD1306 display;

// Класс для управления меню
class Menu
{
private:
    Button *buttonOK, *buttonUP, *buttonDown;
    int currentItem = 0;
    int currendDisplayItem = 0;
    int currentSubmenu = 0;
    MenuType currentTypeField = MENU_SUBMENU;
    TimerMillis tmrUpdate;
    bool tmrHighlightedCursor = false;

public:
    void setup(Button *buttonOK, Button *buttonUP, Button *buttonDown);

    void navigate(void);

    uint8_t isValueChanged(void);

private:
    void displayMenu(void);

    uint8_t getCurrentValueField(int8_t index = -1);

    void displayParameter(int8_t index);
};

extern Menu menu;