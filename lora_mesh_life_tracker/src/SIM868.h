#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include "secret_data.h"

class SIM868

{
private:
    HardwareSerial &mSIM868_UART;
    HardwareSerial &mTerminal_UART;

    String mData_transmitt = "";
    String mData_transmitt_old = "";

    String mTransmittionBuffer = "";
    int mCounter_TX_pack = 0;
    bool mConnect_flag = 1;
    bool mPrevious_power_status = 0;
    int mBuffer_counter = 0;

public:
    int random_piece_upper_limit = 2000;

    SIM868(HardwareSerial &Serial1, HardwareSerial &Serial3);

    // Метод чтения ответа от SIM868
    void read_SIM868();

    // Отправка АТ команды в sim
    void send_AT_command(String command);

    /*
    Отфильтрвываем неправильные данные
    Метод неприменим при отправке из буфера
    */
    void filter_incorrect_data();

    /* Отправляем данные на сервер используя SIM868
        Для удачной отправки необходимо действующее TCP подключение к серверу

        prefix - текст который необходимо вставить паеред сообщением (после prefix вставляется пробел(" "))
        end_of_message - текст который необходимо вставить после сообщения (перед end_of_message вставляется пробел(" "))

        Тело сообщения которое будет отправлено при вызове этотго метода хранится в переменной mDataTransmitt и к ней же
        и будет добавлен префикс и суффикс

        return 1 - если отправлено без ошибок
        return 0 - если при отправке произошла ошибка
    */
    bool send_to_server(String prefix, String end_of_message);

    /*
    Отправляем данные на сервер используя SIM868, без префиксов и окончаний

    message - сообщение которое будет отправлено на сервер

    return 1 - если отправлено без ошибок
    return 0 - если при отправке произошла ошибка
    */
    bool send_to_server(String message);

    /*
    Метод проверки соединения с сервером
    Перед использованием необходимо выполнгить попытку подключения к серверу используя try_connect_to_server(), затем сразу выполняется проверка

    return 1 - удачное подключение
    return 0 - подключение не удалось
    */
    bool check_connect_to_server();

    // выполняем попытку подключиться к серверу
    void try_connect_to_server();

    // настраиваем параметры GPRS (APN)
    void setup_gprs_parameter();

    // включаем GPS
    void PowerUp_gps();

    /*
    Включаем SIM (имитируем нажатие кнопки питания)
    Срабатывает только в том случае если питания до этого на SIM не было и он не отозвался на запрос
    Если SIM от озвался то просто пишет Power ON

    SIM868_PWR_Pin - пин к которому подключен PWRK от SIM модуля
    */
    void power_ON(int SIM868_PWR_Pin);

    /*
    Выключаем SIM (имитируем нажатие кнопки питания)
    Срабатывает только в том случае если питание до этого на SIM было и он отозвался на запрос
    Если SIM не озвался то просто пишет Power OFF

    SIM868_PWR_Pin - пин к которому подключен PWRK от SIM модуля
    */
    void power_OFF(int SIM868_PWR_Pin); // выключаем SIM

    /*
    Получаем телеметрию, отправляем в SIM запрос CGNSINF и после этого распаршиваем на составляющие
    Ключ на этом этапе не добавляется

    Module_ADDR - адрес который необходимо добавить в пакет
    status_count - который необходимо добавить в пакет
    altitude_rate - скороподъемность добавляемая в пакет

    return mData_transmitt
    mData_transmitt = String mData_transmitt =  //
        Module_ADDR + " "     //
        + lattitude + " "     //
        + lontitude + " "     //
        + altitude + " "      //
        + altitude_rate + " " //
        + speed + " "         //
        + status_count + " "  //
        + course;             //
    */
    String get_telemetry(String Module_ADDR, int status_count, String altitude_rate);

    /*
    Получаем минимум телеметрии (широта, долгота, высота) телеметрию, отправляем в SIM запрос CGNSINF и после этого распаршиваем на составляющие
    Ключ на этом этапе не добавляется

    Module_ADDR - адрес который необходимо добавить в пакет

    return mData_transmitt
    mData_transmitt = String mData_transmitt =
        Module_ADDR + " "     //
        + lattitude + " "     //
        + lontitude + " "     //
        + altitude
    */
    String get_telemetry_min(String Module_ADDR);

    /*
    Получаем минимум телеметрии (широта, долгота, высота) телеметрию, отправляем в SIM запрос CGNSINF и после этого распаршиваем на составляющие
    Ключ на этом этапе не добавляется

    Module_ADDR - адрес который необходимо добавить в пакет

    return mData_transmitt
    mData_transmitt = String mData_transmitt =
    lattitude + " "     //
        + lontitude + " "     //
        + altitude
    */
    String get_telemetry_min();

    /*
    Метод для получения строки с телеметрией из класса SIM без чтения данных от GPS
    Метод просто возвращает последние рассчные на момент вызова данные
    Необходимо вызывать до отправки данных на сервер используя методы try_send_to_server() или
    срабатывания отправки буфера через connect_and_send_buffer но после получения телеметрии через get_telemetry()

    return mData_transmitt
    mData_transmitt = String mData_transmitt =  //
        Module_ADDR + " "     //
        + lattitude + " "     //
        + lontitude + " "     //
        + altitude + " "      //
        + altitude_rate + " " //
        + speed + " "         //
        + status_count + " "  //
        + course;             //
    */
    String get_data_transmitt(); // нужно выцзывать до отправки на сервер, т.к после неё обнуляется

    /*
    Выполняем попытку подключения к серверу, проверяем удалось ли подключиться а затем отправляем данные
    в том случае если подключиться удалось

    Использует ключ GL т.е основная посылка
    */
    void try_send_to_server();

    /*
    Выполняем попытку подключения к серверу, проверяем удалось ли подключиться а затем отправляем данные
    в том случае если подключиться удалось

    Использует ключ GV т.е дублирует данные
    */
    void try_send_to_server(bool i);

    /*
        Функция отправки на сервер данных из буфера и пополнения буфера

        message - сообщение которое необходимо добавить в буффер
        length_buffer - длинна буффера, при достижении которой происходит отправка данных на сервер (можно динамически менять)
    */
    void connect_and_send_buffer(String message, int length_buffer);

    /*
    Получаем мощность сигнала от 2G базовой станции и определяем диапазон мощностей. Мощность используемая
    в методе измеряется в попугаях!!! О том как перевести в dBm читай тут: https://repka-pi.ru/blog/post/62#:~:text=%D0%97%D0%BD%D0%B0%D1%87%D0%B5%D0%BD%D0%B8%D0%B5%20%D1%83%D1%80%D0%BE%D0%B2%D0%BD%D1%8F%20%D0%BC%D0%BE%D1%89%D0%BD%D0%BE%D1%81%D1%82%D0%B8,%D0%B8%D0%BB%D0%B8%20%D0%BD%D0%B5%D0%B2%D0%BE%D0%B7%D0%BC%D0%BE%D0%B6%D0%BD%D0%BE%20%D0%BE%D0%BF%D1%80%D0%B5%D0%B4%D0%B5%D0%BB%D0%B8%D1%82%D1%8C

    return 1 - нет связи / неизвестно(невозможно определить)
    return 2 - очень слабый сигнал
    return 3 - слабый сигнал
    return 4 - средний сигнал
    return 5 - хороший сигнал
    return 6 - очень хороший сигнал
    */
    int readBaseStationPowerImage(); // получаем мощность сигнала от базовой станциии определяем что рисовать на экране
};