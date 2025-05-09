#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "configuration.h"

#define COUNT_MENU_ELEMENTS 15
#define COUNT_MENU_PARAMETERS 8
#define COUNT_MENU_INFO 2

enum MenuType
{
    MENU_ITEM,
    MENU_SUBMENU,
    MENU_PARAMETER,
    MENU_INFO
};

struct MenuItem
{
    const char *name; // Название пункта меню
    MenuType type;    // Тип пункта меню
    int parent;       // Индекс родительского пункта (-1, если корень)
    int childCount;   // Количество дочерних элементов
};

struct MenuItemParameter
{
    const int index;         // Индекс пункта меню типа MENU_PARAMETER
    int value;               // Значение
    const int minValue = -1; // Минимальное значение
    const int maxValue = -1; // Максимальное значение
};

struct MenuItemInfo
{
    const int index; // Индекс пункта меню типа MENU_PARAMETER
    char *info;      // Поле информации
};

extern Adafruit_SSD1306 display;
extern const MenuItem menuStruct[];
extern MenuItemParameter menuValueParameter[COUNT_MENU_PARAMETERS];
extern MenuItemInfo menuItemInfo[];

// Класс для управления меню
class Menu
{
private:
    Button *buttonOK, *buttonUP, *buttonDown;
    int currentItem = 0; // Индекс выбранного элемента из menuStruct
    int currendDisplayItem = 0;
    int currentSubmenu = 0;
    MenuType currentTypeField = MENU_SUBMENU;
    TimerMillis tmrUpdate;
    bool tmrHighlightedCursor = false;
    bool flagValueChanged = false;
    int lastValue;
    uint8_t currentValueField = 0;

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