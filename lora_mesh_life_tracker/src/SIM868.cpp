#include "SIM868.h"

SIM868::SIM868(HardwareSerial &Serial1, HardwareSerial &Serial3)
    : mSIM868_UART(Serial3), mTerminal_UART(Serial1) {}

void SIM868::read_SIM868() // функция чтения ответа от SIM868
{
    while (mSIM868_UART.available())
    {
        byte buff123 = mSIM868_UART.read();
        mTerminal_UART.write(buff123);
    }
}

void SIM868::send_AT_command(String command) // отправка АТ команды в sim
{
    mSIM868_UART.println(command);
    read_SIM868();
    // delay(100);
}

bool SIM868::send_to_server(String prefix, String end_of_message) // отправляем данные на сервер используя SIM868
{
    mData_transmitt = prefix + " " + mData_transmitt + " " + end_of_message;
    mTerminal_UART.println("Sending data to server ===>");
    mSIM868_UART.println("AT+CIPSEND=" + String(mData_transmitt.length()));
    mTerminal_UART.print("Sizeof= ");
    mTerminal_UART.println("AT+CIPSEND=" + String(mData_transmitt.length()));
    // delay(100);

    String connection_status = mSIM868_UART.readString();

    mSIM868_UART.println(mData_transmitt); // отправляем пакет // если нету модуля то заменить аргументы в скобках на строку: " 1111 56.45205 84.96131 450 1.5 50 2"
    mTerminal_UART.print("pack = ");
    mTerminal_UART.println(mData_transmitt);

    if (connection_status.indexOf("ERROR") != -1)
    {
        // mPrevious_power_status == 0;
        return (0);
    }
    else
    {

        return (1);
    }
}

bool SIM868::check_connect_to_server() // функция проверки соединения с сервером

{
    bool connect_flag = 0;
    String connect = "connect";
    char CONNECT_buf[150] = "Nothing";
    int CONNECT_buf_index = 0;

    while (mSIM868_UART.available())
    {
        byte buff123 = mSIM868_UART.read();
        mTerminal_UART.write(buff123);
        CONNECT_buf[CONNECT_buf_index] = buff123;
        CONNECT_buf_index++;
    }
    connect = String(CONNECT_buf);
    mTerminal_UART.print("connect = ");
    mTerminal_UART.println(connect);

    if (connect.lastIndexOf("FAIL") != -1)
    {
        mTerminal_UART.println("CONNECT TO SERVER FAIL");
        connect_flag = 0;
    }
    else if (connect.lastIndexOf("CONNECT OK") != -1)
    {
        mTerminal_UART.println("CONNECT TO SERVER OK");
        connect_flag = 1;
    }

    return (connect_flag);
}

void SIM868::try_connect_to_server() // выполняем попытку подключиться к серверу
{
    mSIM868_UART.println("ATE0");
    while (mSIM868_UART.available())
        read_SIM868();
    delay(100);

    // mSIM868_UART.println("AT+CIPCLOSE"); // закрываем старые TCP соединения
    // delay(500);
    // read_SIM868();

    mSIM868_UART.println("AT+CSQ");
    read_SIM868();
    delay(100);

    mSIM868_UART.println("AT+CREG?");
    read_SIM868();
    delay(100);

    mSIM868_UART.println("AT+CGATT?");
    read_SIM868();
    delay(100);

    mSIM868_UART.println("AT+CSTT=\"CMNET\"");
    read_SIM868();
    delay(100);

    mSIM868_UART.println("AT+CIICR");
    delay(100);

    mSIM868_UART.println("AT+CIFSR");
    read_SIM868();

    mSIM868_UART.println(CipStartAddr); // Example: CipStartAddr = "AT+CIPSTART=\"TCP\",\"<ip addres>\",<port>"
    delay(3000);
}

void SIM868::setup_gprs_parameter() // настраиваем ппараметры GPRS (APN)
{
    // delay(15000);
    read_SIM868();
    mSIM868_UART.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    read_SIM868();
    delay(100);
    mSIM868_UART.println("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"");
    read_SIM868();
    delay(100);
    mSIM868_UART.println("AT+SAPBR=1,1");
    read_SIM868();
    delay(100);
}

void SIM868::PowerUp_gps() // включаем GPS
{
    mSIM868_UART.write("AT+CGNSPWR=1\n"); // подаем питание на GPS
    delay(100);
    read_SIM868();
}

void SIM868::power_ON(int SIM868_PWR_Pin) // включаем/выключаем SIM
{
    read_SIM868();
    mSIM868_UART.write("AT\n");
    delay(5);
    String response = mSIM868_UART.readString();

    if (response.indexOf("OK") == -1)
    {
        pinMode(SIM868_PWR_Pin, OUTPUT);
        digitalWrite(SIM868_PWR_Pin, HIGH);
        delay(100);
        digitalWrite(SIM868_PWR_Pin, LOW);
        delay(1000);
        digitalWrite(SIM868_PWR_Pin, HIGH);
        delay(3000);
        pinMode(SIM868_PWR_Pin, INPUT);

        mTerminal_UART.println("Power ON");

        // mPrevious_power_status = 0;
    }
}

void SIM868::power_OFF(int SIM868_PWR_Pin) // включаем/выключаем SIM
{
    read_SIM868();
    mSIM868_UART.write("AT\n");
    delay(5);
    String response = mSIM868_UART.readString();

    if (response.indexOf("OK") != -1)
    {
        pinMode(SIM868_PWR_Pin, OUTPUT);
        digitalWrite(SIM868_PWR_Pin, HIGH);
        delay(100);
        digitalWrite(SIM868_PWR_Pin, LOW);
        delay(1000);
        digitalWrite(SIM868_PWR_Pin, HIGH);
        delay(3000);
        pinMode(SIM868_PWR_Pin, INPUT);

        mTerminal_UART.println("Power OFF");
    }
}

void SIM868::filter_incorrect_data()
{
    if (mData_transmitt == "")
    {
        mData_transmitt = mData_transmitt_old;
    }
    if (mData_transmitt_old.indexOf("E") == -1)
    { // отбраковываем данные которые содержат ешки
        mData_transmitt = mData_transmitt_old;
    }
    mTerminal_UART.println(mData_transmitt);
}

String SIM868::get_telemetry(String Module_ADDR, int status_count, String altitude_rate) // получаем телеметрию
{
    String lattitude = "lattitude";
    String lontitude = "lontitude";
    String altitude = "altitude";
    String speed = "speed";
    String course = "course";
    char GPS_buff[150] = "Nothing";
    int GPS_buff_index = 0;
    int index1 = 0;
    String GPS_str = "GPS";

    // =============================== ПОЛУЧЕНИЕ ТЕЛЕМЕТРИИ ==============================
    mTerminal_UART.print("Get GPS:");

    read_SIM868(); // на всякиий случай, перед получением корд читаем юарт, чтобы буфер был гарантированно пуст
    mSIM868_UART.write("AT+CGNSINF\n");

    delay(5);
    while (mSIM868_UART.available())
    {
        GPS_str = mSIM868_UART.readString();
        mTerminal_UART.println(GPS_str);
    }
    // GPS_str = "1,1,20240208183233.000,-90.000000,-180.000000,336.55,0.00,323.0,1,,0.9,1.2,0.8,,12,10,9,,33,,";//подмена для отладки

    GPS_buff_index = 0;

    index1 = (GPS_str.indexOf(".") + 5);
    GPS_str = GPS_str.substring(index1);

    lattitude = GPS_str.substring(0, GPS_str.indexOf(","));
    lontitude = GPS_str.substring(GPS_str.indexOf(",") + 1);
    altitude = lontitude.substring(lontitude.indexOf(",") + 1);
    speed = altitude.substring(altitude.indexOf(",") + 1);
    course = speed.substring(speed.indexOf(",") + 1);

    lontitude = lontitude.substring(0, lontitude.indexOf(","));
    altitude = altitude.substring(0, altitude.indexOf(","));
    speed = speed.substring(0, speed.indexOf(","));
    course = course.substring(0, course.indexOf(","));

    lontitude = lontitude.substring(0, lontitude.indexOf(".") + 7); // чтобы обрезать до нужного количества знаков после точки указывать нужно на 1 больше
    lattitude = lattitude.substring(0, lattitude.indexOf(".") + 7); // чтобы обрезать до нужного количества знаков после точки указывать нужно на 1 больше

    if (lattitude.length() <= 6)
    {
        lattitude = "E";
    }
    if (lontitude.length() <= 6)
    {
        lontitude = "E";
    }
    if (altitude.length() < 3)
    {
        altitude = "E";
    }
    if ((speed.length() < 3) or (speed.startsWith(".") == true))
    {
        speed = "E";
    }
    if (course.length() < 1)
    {
        course = "E";
    }
    String mData_transmitt =  //
        Module_ADDR + " "     //
        + lattitude + " "     //
        + lontitude + " "     //
        + altitude + " "      //
        + altitude_rate + " " //
        + speed + " "         //
        + status_count + " "  //
        + course;             //+ " " + router_hop;

    mData_transmitt_old = mData_transmitt;
    return (mData_transmitt);
}

void SIM868::try_send_to_server() // отправляем данные на сервер и проверяем статус подключения
{
    if ((mConnect_flag == 0) /*and (mPrevious_power_status == 0)*/)
    {
        try_connect_to_server();                   // пытаемя подключиьтся к серверу
        mConnect_flag = check_connect_to_server(); // проверяем получилось ли подключиться
        //get_telemetry();
    }

    if (mConnect_flag == 1)
    {
        mCounter_TX_pack++;
        mConnect_flag = send_to_server("GL", String(mCounter_TX_pack)); // если получилось подключиться то отправляем данные

        mData_transmitt = "";
    }
}

String SIM868::get_data_transmitt()
{
    return (mData_transmitt);
}

int SIM868::readBaseStationPowerImage()
{
    read_SIM868();
    mSIM868_UART.write("AT+CSQ\n");
    while (!mSIM868_UART.available())
    {
    };
    String response = mSIM868_UART.readString();

    response = response.substring(response.indexOf(":") + 2);
    response = response.substring(0, response.indexOf(","));

    int response_int = response.toInt();
    mTerminal_UART.print("base_signal_PWR =");
    mTerminal_UART.println(response);
    mTerminal_UART.print("response_int =");
    mTerminal_UART.println(response_int);

    if ((response_int == 1) or (response_int == 0))
    {
        return (1); // нет связи
    }
    else if (response_int == 1)
    {
        return (2); // очень слабый сигнал
    }
    else if ((2 <= response_int) and (response_int < 5))
    {
        return (3); // слабый сигнал
    }
    else if ((5 <= response_int) and (response_int < 12))
    {
        return (4); // средний сигнал
    }
    else if ((12 <= response_int) and (response_int < 20))
    {
        return (5); // хороший сигнал
    }
    else if ((20 <= response_int) and (response_int <= 31))
    {
        return (6); // очень хороший сигнал
    }
    else if (response_int == 99)
    {
        return (1); // неизвестно
    }
    else
    {
        return (0);
    }
};
