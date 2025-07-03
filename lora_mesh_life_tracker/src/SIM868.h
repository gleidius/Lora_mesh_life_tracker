#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

class SIM868

{
private:
    HardwareSerial &MySerial3;
    HardwareSerial &MySerial1;

public:
    int random_piece_upper_limit = 2000;

    SIM868(HardwareSerial &Serial1, HardwareSerial &Serial3);

    void read_SIM868(); // функция чтения ответа от SIM868

    void send_SIM868(String command); // отправка АТ команды в sim

    bool send_to_server_SIM868(String dataTransmit); // отправляем данные на сервер используя SIM868

    bool check_connect_to_server(); // функция проверки соединения с сервером

    void try_connect_to_server(); // выполняем попытку подключиться к серверу

    void setup_gprs_parameter(); // настраиваем ппараметры GPRS (APN)

    void SIM868_GPS_Power_Up(); // включаем GPS

    void SIM868_Power_SW(int SIM868_PWR_Pin); // включаем/выключаем Е52

    String get_telemetry(String Module_ADDR, int status_count, String altitude_rate); // получаем телеметрию
};