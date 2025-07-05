#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

class SIM868

{
private:
    HardwareSerial &mSIM868_UART;
    HardwareSerial &mTerminal_UART;

    String mData_transmitt = "";
    String mData_transmitt_old = "";
    int mCounter_TX_pack = 0;
    bool mConnect_flag = 0;

public:
    int random_piece_upper_limit = 2000;

    SIM868(HardwareSerial &Serial1, HardwareSerial &Serial3);

    void read_SIM868(); // функция чтения ответа от SIM868

    void send_AT_command(String command); // отправка АТ команды в sim

    void filter_incorrect_data(); // отфильтрвываем неправильные данные

    bool send_to_server(String prefix, String end_of_message); // отправляем данные на сервер используя SIM868

    bool check_connect_to_server(); // функция проверки соединения с сервером

    void try_connect_to_server(); // выполняем попытку подключиться к серверу

    void setup_gprs_parameter(); // настраиваем ппараметры GPRS (APN)

    void PowerUp_gps(); // включаем GPS

    void power_ON(int SIM868_PWR_Pin);  // включаем SIM
    void power_OFF(int SIM868_PWR_Pin); // выключаем SIM

    String get_telemetry(String Module_ADDR, int status_count, String altitude_rate); // получаем телеметрию

    void try_send_to_server(); // пытаемся отправить данные на сервер
};