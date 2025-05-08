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
public:
    void setup(Button *buttonOK, Button *buttonUP, Button *buttonDown);

    void navigate()
    {
        displayMenu();
        if (buttonUP->click())
        {
            Serial.println("UP");
            switch (currentTypeField)
            {
            case MENU_SUBMENU:
                if (currendDisplayItem < menuStruct[currentSubmenu].childCount - 1)
                    currendDisplayItem++;
                break;

            case MENU_PARAMETER:
                uint8_t index = getCurrentValueField();
                if (menuValueParameter[index].value > menuValueParameter[index].minValue)
                    menuValueParameter[index].value--;
                break;
            }
        }
        if (buttonDown->click())
        {
            switch (currentTypeField)
            {
            case MENU_SUBMENU:
                if (currendDisplayItem > 0)
                    currendDisplayItem--;
                break;

            case MENU_PARAMETER:
                uint8_t index = getCurrentValueField();
                if (menuValueParameter[index].value < menuValueParameter[index].maxValue)
                    menuValueParameter[index].value++;
                break;
            }
        }

        if (buttonOK->click())
        {
            switch (menuStruct[currentItem].type)
            {
            case MENU_SUBMENU:
                currentSubmenu = currentItem;
                break;

            case MENU_PARAMETER:
                currentTypeField = (currentTypeField == MENU_PARAMETER) ? MENU_SUBMENU : MENU_PARAMETER;
                break;
            }
        }
    }

private:
    Button *buttonOK, *buttonUP, *buttonDown;
    int currentItem = 0;
    int currendDisplayItem = 0;
    int currentSubmenu = 0;
    MenuType currentTypeField = MENU_SUBMENU;

    void displayMenu()
    {
        display.clearDisplay();
        display.setCursor(0, 0);

        for (uint8_t i = currentSubmenu, j = 0; (i < COUNT_MENU_ELEMENTS) && (j < menuStruct[currentSubmenu].childCount); i++)
        {
            if (menuStruct[i].parent == currentSubmenu)
            {
                if (currendDisplayItem == j)
                {
                    currentItem = i;
                    display.setTextColor(BLACK, WHITE); // Инверсия цвета для выделенного пункта
                }
                else
                    display.setTextColor(WHITE);

                if (menuStruct[i].type == MENU_PARAMETER)
                {
                    display.print(menuStruct[i].name);
                    display.print(": ");
                    display.println(menuValueParameter[getCurrentValueField(i)].value);
                }
                else
                    display.println(menuStruct[i].name); // Отображаем имя параметра
                j++;
            }
        }
        display.display();
    }

    uint8_t getCurrentValueField(int8_t index = -1)
    {
        index = (index != -1) ? index : currentItem;
        for (uint8_t i = 0; (i < COUNT_MENU_PARAMETERS); i++)
            if (menuValueParameter[i].index == index)
                return i;
        return 0;
    }
};
