#pragma once

enum MenuType
{
    MENU_ITEM,
    MENU_SUBMENU,
    MENU_PARAMETER
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

#define COUNT_MENU_ELEMENTS 6

const MenuItem menuStruct[] = {
    {"root", MENU_SUBMENU, -1, 3},              // 0
    /**/ {"Settings", MENU_SUBMENU, 0, 2},      // 1
    /*    */ {"bridges", MENU_PARAMETER, 1, 0}, // 2
    /*    */ {"sound", MENU_PARAMETER, 1, 0},   // 3
    /**/ {"Exit", MENU_ITEM, 0, 0},             // 4
    /**/ {"Test", MENU_SUBMENU, 0, 0},          // 5
};

#define COUNT_MENU_PARAMETERS 2

static MenuItemParameter menuValueParameter[] = {
    {2, 4, 3, 9}, // bridges
    {3, 3, 2, 11} // sound
};