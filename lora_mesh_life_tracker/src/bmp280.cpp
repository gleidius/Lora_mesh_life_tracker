#include "bmp280.h"
#include "variables.h"

// bmp280::bmp280()
// {
//     bmp.begin(0x76);
//     bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
//                     Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
//                     Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
//                     Adafruit_BMP280::FILTER_X16,      /* Filtering. */
//                     Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
// }
void bmp280::begin()
{
    bmp.begin(0x76);
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

float bmp280::get_altitude_rate(float P, float P_pred, int t, int t_pred) // получаем скороподъемность
{
    float R = 8.134;                          // газовая постоянная
    float T = bmp.readTemperature() + 273.15; // температура в кельвинах
    float g = 9.81;                           // ускорение свободного падения
    float M = 0.029;                          // молярная масса воздуха

    return ((((R * T) / (g * M)) * ((P_pred - P) / (P * ((t / 1000) - (t_pred / 1000))))));
}

String bmp280::get_ar_with_filter(int ALTR_Xpos, int ALTR_Ypos) // получаем и фильтруем скороподъемность
{
    alt_rate_time = millis();

    time_massiv[0] = time_massiv[1];
    time_massiv[1] = millis();
    Preshure[0] = Preshure[1];
    Preshure[1] = bmp.readPressure();

    alt_rate_massiv[4] = alt_rate_massiv[3];
    alt_rate_massiv[3] = alt_rate_massiv[2];
    alt_rate_massiv[2] = alt_rate_massiv[1];
    alt_rate_massiv[1] = alt_rate_massiv[0]; // криворукий циклический буффер !!! исправить
    alt_rate_massiv[0] = get_altitude_rate(Preshure[1], Preshure[0], time_massiv[1], time_massiv[0]);

    float alt_rate = ((alt_rate_massiv[0] + alt_rate_massiv[1] + alt_rate_massiv[2] + alt_rate_massiv[3] + alt_rate_massiv[4]) / 5);

    // draw_pos(ALTR_Xpos, ALTR_Ypos, String(alt_rate));
    // altitude_rate = String(alt_rate);

    return (String(alt_rate));
}