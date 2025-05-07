#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "configuration.h"

extern Adafruit_SSD1306 display;

// Структура для узла меню
enum MenuItemType
{
    MENU_ITEM,
    PARAMETER_ITEM
};

struct MenuItemParameter
{
    uint16_t *parameterValue;
    uint16_t maxValue;
    uint16_t minValue;
};

struct MenuItem
{
    const char *name;
    MenuItemType type;
    MenuItem *next;
    MenuItem *prev;
    MenuItem *submenu;            // Указатель на подменю
    MenuItemParameter *parameter; // Указатель на параметр (если это параметр)
};

// Класс для управления меню
class Menu
{
public:
    Menu() : head(nullptr), currentItem(nullptr) {}

    void setup();

    MenuItem *addMenuItem(const char *name)
    {
        return addItem(name, MENU_ITEM, nullptr);
    }

    void addParameterItem(const char *name, uint16_t *parameterValue, uint16_t minValue, uint16_t maxValue)
    {
        MenuItemParameter *itemParameter = new MenuItemParameter();
        itemParameter->parameterValue = parameterValue;
        itemParameter->maxValue = maxValue;
        itemParameter->minValue = minValue;

        addItem(name, PARAMETER_ITEM, itemParameter);
    }

    void setSubmenu(MenuItem *item)
    {
        item->submenu = head; // Привязываем подменю к элементу
    }

    void navigate()
    {
        currentItem = head;
        int selected = 0;

        while (true)
        {
            switch (flagParameter)
            {
            case MENU_ITEM:
            {
                displayMenu(selected);

                if (digitalRead(buttonUpPin) == LOW)
                {
                    currentItem = (currentItem->prev != nullptr) ? currentItem = currentItem->prev : currentItem = tail;
                    selected = (selected - 1 + countItems()) % countItems(); // Циклический выбор вверх
                    indexStr = selected;
                    delay(200); // Антидребезг
                }
                if (digitalRead(buttonDownPin) == LOW)
                {
                    currentItem = (currentItem->next != nullptr) ? currentItem = currentItem->next : currentItem = head;
                    selected = (selected + 1) % countItems(); // Циклический выбор вниз
                    indexStr = selected;
                    delay(200); // Антидребезг
                }

                if (digitalRead(buttonSelectPin) == LOW)
                {
                    if (currentItem->type == MENU_ITEM && currentItem->submenu != nullptr)
                    {
                        // Переход в подменю при выборе элемента
                        navigateSubmenu(currentItem->submenu);
                    }
                    else if (currentItem->type == PARAMETER_ITEM)
                    {
                        handleParameter();
                    }

                    delay(200); // Антидребезг
                }
                break;
            }
            case PARAMETER_ITEM:
            {
                if (digitalRead(buttonUpPin) == LOW)
                {
                    if (*currentItem->parameter->parameterValue != currentItem->parameter->minValue)
                        (*currentItem->parameter->parameterValue)--;
                    delay(200); // Антидребезг
                }
                if (digitalRead(buttonDownPin) == LOW)
                {
                    if (*currentItem->parameter->parameterValue != currentItem->parameter->maxValue)
                        (*currentItem->parameter->parameterValue)++;

                    delay(200); // Антидребезг
                }

                if (digitalRead(buttonSelectPin) == LOW)
                {
                    flagParameter = MENU_ITEM;
                    delay(200); // Антидребезг
                }
                changeParameterValue();
                break;
            }
            }
        }
    }

private:
    MenuItem *head;
    MenuItem *tail;
    MenuItem *currentItem;
    MenuItemType flagParameter = MENU_ITEM;
    uint8_t indexStr = 0;

    MenuItem *addItem(const char *name, MenuItemType type, MenuItemParameter *itemParameter)
    {
        MenuItem *newItem = new MenuItem;
        newItem->name = name;
        newItem->type = type;
        newItem->next = nullptr;
        newItem->submenu = nullptr;         // Изначально подменю пустое
        newItem->parameter = itemParameter; // Устанавливаем указатель на параметр

        if (head == nullptr)
        {
            newItem->prev = nullptr;
            head = newItem; // Если это первый элемент
            tail = newItem;
        }
        else
        {
            MenuItem *current = tail;

            current->next = newItem; // Добавляем новый элемент в конец списка
            newItem->prev = current; // Устанавливаем связь с предыдущим элементом

            tail = newItem;
        }
        return newItem;
    }

    void displayMenu(int selected)
    {
        display.clearDisplay();
        MenuItem *currentItem = head;
        int index = 0;

        while (currentItem != nullptr && index < 3)
        { // Отображаем максимум 3 пункта меню
            if (index == selected)
            {
                display.setTextColor(BLACK, WHITE); // Инверсия цвета для выделенного пункта
            }
            else
            {
                display.setTextColor(WHITE);
            }

            display.setCursor(0, index * 10);
            display.print(currentItem->name); // Отображаем имя параметра

            // Проверяем, является ли элемент параметром и отображаем его значение
            if (currentItem->type == PARAMETER_ITEM && currentItem->parameter->parameterValue != nullptr)
            {
                display.print(": ");                                    // Разделитель между именем и значением
                display.print(*currentItem->parameter->parameterValue); // Отображаем значение параметра
            }

            currentItem = currentItem->next;
            index++;
        }

        display.display();
    }

    void navigateSubmenu(MenuItem *submenu)
    {
        Menu subMenu;
        subMenu.head = submenu; // Устанавливаем голову подменю
        subMenu.navigate();     // Запускаем навигацию по подменю
    }

    void handleParameter()
    {
        flagParameter = PARAMETER_ITEM;
        uint8_t textWidth = (strlen(currentItem->name) + 2) * 6;
        uint8_t textHeight = indexStr * 10;

        display.setCursor(0, textHeight);
        display.fillRect(0, textHeight, 128, 8, SSD1306_BLACK);

        display.setTextColor(WHITE);
        display.print(currentItem->name); // Отображаем имя параметра
        display.print(": ");

        display.setTextColor(BLACK, WHITE);
        display.print(*currentItem->parameter->parameterValue);
        display.display();
    }

    void changeParameterValue()
    {

        uint8_t textWidth = (strlen(currentItem->name) + 2) * 6;
        uint8_t textHeight = indexStr * 10;
        display.setTextColor(BLACK, WHITE);
        display.fillRect(textWidth, textHeight, 128, 8, SSD1306_BLACK);
        display.setCursor(textWidth, textHeight);

        display.print(*currentItem->parameter->parameterValue);
        display.display();
    }

    void drawPos(int x_pos, int y_pos, char *text);
    void drawPos(int x_pos, int y_pos, int &text);

    int countItems()
    {
        int count = 0;
        MenuItem *current = head;
        while (current != nullptr)
        {
            count++;
            current = current->next;
        }
        return count;
    }
};
