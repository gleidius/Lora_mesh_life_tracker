#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

class e52
{
private:
    HardwareSerial &S_Serial;
    HardwareSerial &MySerial1;

public:
    e52(HardwareSerial &Serial1, HardwareSerial &Serial3);

    void send_command(String command); // функиця отправки AT-команды в Е52

    void set_power(int power); // функция настройки мощности

    void read_SSerial(); // функция чтения Soft UART с задержкой

    int set_pause(int pause); // функция установки паузы передачи

    void set_rs(int rs); // функция изменения параметра скорость/дальность

    void set_SRC_ADDR(int SRC); // функция изменения собственного адреса

    String Set_E52_ADDR(); // устанавливаем адрес Е52 по последним 4-м ицфрам МАС адреса

    void send_to_mesh_E52(String data_transmitt); // отправляем данные в меш при помщи Е52

    void E52_default_init(); // инициализируемся по дефолту

    String read_router_hop(); // считываем номер модуля через который произошел hop
};