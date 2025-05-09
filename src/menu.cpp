#include "menu.h"

const MenuItem menuStruct[] = {
    {"Mouse Board", MENU_SUBMENU, -1, 2},               // 0
    /**/ {"Settings", MENU_SUBMENU, 0, 3},              // 1
    /*    */ {"Mesh", MENU_SUBMENU, 1, 5},              // 2
    /*        */ {"Power: ", MENU_PARAMETER, 2, 0},     // 3
    /*        */ {"Rate: ", MENU_PARAMETER, 2, 0},      // 4
    /*        */ {"Option: ", MENU_PARAMETER, 2, 0},    // 5
    /*        */ {"Dest address: ", MENU_INFO, 2, 0},   // 6
    /*        */ {"Source address: ", MENU_INFO, 2, 0}, // 7
    /*    */ {"Network", MENU_SUBMENU, 1, 4},           // 8
    /*        */ {"bridges", MENU_PARAMETER, 8, 0},     // 9
    /*        */ {"sound", MENU_PARAMETER, 8, 0},       // 10
    /*        */ {"test 1", MENU_PARAMETER, 8, 0},      // 11
    /*        */ {"test 2", MENU_PARAMETER, 8, 0},      // 12
    /*    */ {"WiFi", MENU_SUBMENU, 1, 2},              // 13
    /**/ {"bool", MENU_PARAMETER, 0, 0},                // 14
};

uint16_t srcAddr;
uint16_t dstAddr;
MenuItemParameter menuValueParameter[] = {
    {3, 16, -8, 22}, // Power
    {4, 0, 0, 2},    // Rate
    {5, 1, 1, 4},    // Option
    {9, 4, 3, 9},    // bridges
    {10, 4, 3, 9},   // sound
    {11, 4, 3, 9},   // test 1
    {12, 3, 2, 11},  // test 2
    {14, 0, 0, 0}    // bool
};

MenuItemInfo menuItemInfo[] = {
    {6, nullptr}, // Dest address LoRa
    {7, nullptr}  // Source address LoRa
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
        switch (currentTypeField)
        {
        case MENU_SUBMENU:
            if (currendDisplayItem < menuStruct[currentSubmenu].childCount)
                currendDisplayItem++;
            break;

        case MENU_PARAMETER:
            uint8_t index = getCurrentValueField();
            if (menuValueParameter[index].value > menuValueParameter[index].minValue || // Обычные параметры
                (menuValueParameter[index].minValue == 0 &&                             // Булев параметр
                 menuValueParameter[index].maxValue == 0 &&
                 menuValueParameter[index].value != 0))
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
            if (menuValueParameter[index].value < menuValueParameter[index].maxValue || // Обычные параметры
                (menuValueParameter[index].minValue == 0 &&                             // Булев параметр
                 menuValueParameter[index].maxValue == 0 &&
                 menuValueParameter[index].value != 1))
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
            if (currentSubmenu == currentItem && menuStruct[currentItem].parent != -1)
            {
                currentItem = currentSubmenu;
                currendDisplayItem = 0;
                for (uint8_t index = menuStruct[currentItem].parent; index < COUNT_MENU_ELEMENTS; index++)
                {
                    if (menuStruct[index].parent == menuStruct[currentItem].parent)
                    {
                        currendDisplayItem++;
                        if (menuStruct[index].name == menuStruct[currentItem].name)
                            break;
                    }
                }
                currentSubmenu = menuStruct[currentItem].parent;
            }
            else
            {
                currendDisplayItem = 0;
                currentSubmenu = currentItem;
            }

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

    for (uint8_t index = currentSubmenu, j = 0; (index < COUNT_MENU_ELEMENTS) && (j < menuStruct[currentSubmenu].childCount + 1); index++)
    {
        if (menuStruct[index].parent == currentSubmenu || index == currentSubmenu)
        {
            if (currendDisplayItem == j)
            {
                currentItem = index;
                display.setTextColor(BLACK, WHITE); // Инверсия цвета для выделенного пункта
            }
            else
                display.setTextColor(WHITE);

            switch (menuStruct[index].type)
            {

            case MENU_SUBMENU:
                if (index == currentSubmenu)
                {
                    display.println(menuStruct[currentSubmenu].name);
                    display.drawFastHLine(0, display.getCursorY() + 2, SCREEN_WIDTH, WHITE);
                    display.setCursor(0, display.getCursorY() + 5);
                }
                else
                    display.println(menuStruct[index].name); // Отображаем имя параметра
                break;
            case MENU_PARAMETER:
                displayParameter(index);
                break;
            case MENU_INFO:
                display.print(menuStruct[index].name);
                for (uint8_t i = 0; (i < COUNT_MENU_INFO); i++)
                    if (menuItemInfo[i].index == index)
                        display.println(menuItemInfo[i].info);
                break;
            }
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
    if (menuValueParameter[getCurrentValueField(index)].minValue == 0 && menuValueParameter[getCurrentValueField(index)].maxValue == 0)
        if (menuValueParameter[getCurrentValueField(index)].value)
            display.println("On");
        else
            display.println("Off");
    else
        display.println(menuValueParameter[getCurrentValueField(index)].value);
}
