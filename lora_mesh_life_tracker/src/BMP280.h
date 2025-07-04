#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_BMP280.h>

class BMP280
{
private:
    Adafruit_BMP280 mBMP;

    float mPreshure[2]{101325, 101325};
    float mAlt_rate_massiv[5]{0, 0, 0, 0, 0};
    int mTime_massiv[2]{0, 0};
    unsigned long mAlt_rate_time = millis();

public:
    void begin(void);

    float get_altitude_rate(float pressure, float previousPressure, int time, int previousTime); // получаем скороподъемность

    String get_ar_with_filter(); // получаем и фильтруем скороподъемность
};