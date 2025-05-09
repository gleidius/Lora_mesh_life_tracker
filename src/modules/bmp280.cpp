#include "bmp280.h"

BMP280 bmp280;

bool BMP280::begin()
{
    return bmp.begin(BMP_ADDRESS); // Инициализация BMP280
}

void BMP280::update()
{
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 1000)
    { // обновление каждую секунду
        float currentPressure = bmp.readPressure();
        float altitude = bmp.readAltitude(1013.25); // Стандартное давление на уровне моря

        // Фильтрация данных
        pressureSum += currentPressure;
        count++;
        float averagePressure = pressureSum / count;

        if (lastPressure != 0)
        {
            float pressureChange = averagePressure - lastPressure;
            // Переводим изменение давления в метры
            float heightChange = (pressureChange / 100.0) * 8.43; // 1 гПа ~ 8.43 м

            // Учитываем погрешность (например, 10%)
            float errorMargin = 0.1;           // 10%
            heightChange *= (1 - errorMargin); // Уменьшаем на погрешность

            // Устанавливаем порог для значимого изменения высоты
            if (abs(heightChange) > 0.1)
            { // Порог в 0.1 метра
                // Рассчитываем скорость подъема в метрах в секунду
                ascentRate = heightChange / ((currentTime - lastTime) / 1000.0); // м/с
            }
            else
            {
                ascentRate = 0; // Если изменение незначительное, скорость подъема равна 0
            }
        }
        lastPressure = averagePressure;
        lastTime = currentTime;

        // Отображение информации
        Serial.print("Current Altitude: ");
        Serial.print(altitude);
        Serial.print(" m, Ascent Rate: ");
        Serial.print(ascentRate);
        Serial.println(" m/s");

        dtostrf(altitude, 6, 2, result);
        menuItemInfo[2].info = result;
    }
}