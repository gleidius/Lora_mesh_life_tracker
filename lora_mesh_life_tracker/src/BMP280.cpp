#include "BMP280.h"
#include "variables.h"

void BMP280::begin()
{
    mBMP.begin(0x76);
    mBMP.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                     Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                     Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                     Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                     Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

float BMP280::get_altitude_rate(float pressure, float previousPressure, int time, int previousTime) // получаем скороподъемность
{
    float R = 8.134;                           // газовая постоянная
    float T = mBMP.readTemperature() + 273.15; // температура в кельвинах
    float g = 9.81;                            // ускорение свободного падения
    float M = 0.029;                           // молярная масса воздуха

    return ((((R * T) / (g * M)) * ((previousPressure - pressure) / (pressure * ((time / 1000) - (previousTime / 1000))))));
}

String BMP280::get_ar_with_filter() // получаем и фильтруем скороподъемность
{
    mAlt_rate_time = millis();

    mTime_massiv[0] = mTime_massiv[1];
    mTime_massiv[1] = millis();
    mPreshure[0] = mPreshure[1];
    mPreshure[1] = mBMP.readPressure();

    mAlt_rate_massiv[4] = mAlt_rate_massiv[3];
    mAlt_rate_massiv[3] = mAlt_rate_massiv[2];
    mAlt_rate_massiv[2] = mAlt_rate_massiv[1];
    mAlt_rate_massiv[1] = mAlt_rate_massiv[0]; // криворукий циклический буффер !!! исправить
    mAlt_rate_massiv[0] = get_altitude_rate(mPreshure[1], mPreshure[0], mTime_massiv[1], mTime_massiv[0]);

    float alt_rate = ((mAlt_rate_massiv[0] + mAlt_rate_massiv[1] + mAlt_rate_massiv[2] + mAlt_rate_massiv[3] + mAlt_rate_massiv[4]) / 5);

    return (String(alt_rate));
}