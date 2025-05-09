#pragma once
#include "configuration.h"

#include <Adafruit_BMP280.h>

class BMP280
{
public:
    BMP280() : bmp(), lastPressure(0), lastTime(0), ascentRate(0), pressureSum(0), count(0) {}

    bool begin();

    void update();

private:
    char result[8]; // Buffer big enough for 7-character float

    Adafruit_BMP280 bmp;
    float lastPressure;
    unsigned long lastTime;
    float ascentRate;

    float pressureSum; // Сумма давлений для фильтрации
    int count;         // Количество измерений
};

extern BMP280 bmp280;