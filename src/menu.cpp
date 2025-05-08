#include "menu.h"

const MenuItem menuStruct[] = {
    {"root", MENU_SUBMENU, -1, 4},              // 0
    /**/ {"Settings", MENU_SUBMENU, 0, 2},      // 1
    /*    */ {"bridges", MENU_PARAMETER, 1, 0}, // 2
    /*    */ {"sound", MENU_PARAMETER, 1, 0},   // 3
    /**/ {"Exit", MENU_ITEM, 0, 0},             // 4
    /**/ {"Test", MENU_SUBMENU, 0, 0},          // 5
    /**/ {"sound", MENU_PARAMETER, 0, 0},       // 6
};

MenuItemParameter menuValueParameter[] = {
    {2, 4, 3, 9},  // bridges
    {3, 3, 2, 11}, // sound
    {6, 7, -8, 22} // power
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Menu menu;

// public method
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

    tmrUpdate.setInterval(500);
    tmrUpdate.restart();
}

void Menu::navigate(void)
{
    if (tmrUpdate.isReady())
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
        displayMenu();
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
        displayMenu();
    }

    if (buttonOK->click())
    {
        switch (menuStruct[currentItem].type)
        {
        case MENU_SUBMENU:
            currentSubmenu = currentItem;
            break;

        case MENU_PARAMETER:
            if (currentTypeField == MENU_PARAMETER)
            {
                currentTypeField = MENU_SUBMENU;
                if (lastValue != menuValueParameter[getCurrentValueField()].value)
                    flagValueChanged = true;
            }
            else
            {
                currentTypeField = MENU_PARAMETER;
                lastValue = menuValueParameter[getCurrentValueField()].value;
            }

            break;
        }
        displayMenu();
    }
}

// Возвращает индекс элемента массива menuValueParameter который был изменен
// если не было изменений вернет 255
uint8_t Menu::isValueChanged(void)
{
    bool changed = false;
    if (flagValueChanged)
        changed = true;
    flagValueChanged = false;
    if (changed)
        return getCurrentValueField();
    else
        return 255;
}

// private method
void Menu::displayMenu(void)
{
    tmrUpdate.reset();
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
                displayParameter(i);
            else
                display.println(menuStruct[i].name); // Отображаем имя параметра
            j++;
        }
    }
    display.display();
}

uint8_t Menu::getCurrentValueField(int8_t index)
{
    index = (index != -1) ? index : currentItem;
    for (uint8_t i = 0; (i < COUNT_MENU_PARAMETERS); i++)
        if (menuValueParameter[i].index == index)
            return i;
    return 0;
}

void Menu::displayParameter(int8_t index)
{
    if (currentTypeField == MENU_PARAMETER && index == currentItem)
        tmrHighlightedCursor ^= 1;

    if (currentTypeField == MENU_PARAMETER && index == currentItem)
        display.setTextColor(WHITE);
    display.print(menuStruct[index].name);
    display.print(": ");

    if (currentTypeField == MENU_PARAMETER && index == currentItem && tmrHighlightedCursor)
        display.setTextColor(BLACK, WHITE);
    display.setCursor((strlen(menuStruct[index].name) + 2) * 6, display.getCursorY());
    display.println(menuValueParameter[getCurrentValueField(index)].value);
}
