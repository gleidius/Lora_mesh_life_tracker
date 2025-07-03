#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_BMP280.h>

class bmp280
{
private:
    Adafruit_BMP280 bmp;

    float Preshure[2]{101325, 101325};
    float alt_rate_massiv[5]{0, 0, 0, 0, 0};
    int time_massiv[2]{0, 0};
    // String altitude_rate = "E";
    unsigned long alt_rate_time = millis();

public:
    // bmp280();
    void begin(void);

    float get_altitude_rate(float P, float P_pred, int t, int t_pred); // получаем скороподъемность

    String get_ar_with_filter(int ALTR_Xpos, int ALTR_Ypos); // получаем и фильтруем скороподъемность
};