#include "SIM868.h"

SIM868::SIM868(HardwareSerial &Serial1, HardwareSerial &Serial3)
    : MySerial3(Serial3), MySerial1(Serial1) {}

void SIM868::read_SIM868() // функция чтения ответа от SIM868
{
    while (MySerial3.available())
    {
        byte buff123 = MySerial3.read();
        MySerial1.write(buff123);
    }
}

void SIM868::send_SIM868(String command) // отправка АТ команды в sim
{
    MySerial3.println(command);
    read_SIM868();
    delay(100);
}

bool SIM868::send_to_server_SIM868(String dataTransmit) // отправляем данные на сервер используя SIM868
{
    MySerial1.println("Sending data to server ===>");
    MySerial3.println("AT+CIPSEND=" + String(dataTransmit.length()));
    MySerial1.print("Sizeof= ");
    MySerial1.println("AT+CIPSEND=" + String(dataTransmit.length()));
    delay(100);

    String connection_status = MySerial3.readString();

    /*
    MySerial1.print("Connection Status =");
    MySerial1.println(connection_status);
    */

    // read_SIM868();
    MySerial3.println(dataTransmit); // отправляем пакет // если нету модуля то заменить аргументы в скобках на строку: " 1111 56.45205 84.96131 450 1.5 50 2"
    MySerial1.print("pack = ");
    MySerial1.println(dataTransmit);

    if (connection_status.indexOf("ERROR") != -1)
    {
        return (0);
    }
    else
    {
        return (1);
    }

    // read_SIM868();
}

bool SIM868::check_connect_to_server() // функция проверки соединения с сервером

{
    bool connect_flag = 0;
    String connect = "connect";
    char CONNECT_buf[150] = "Nothing";
    int CONNECT_buf_index = 0;

    while (MySerial3.available())
    {
        byte buff123 = MySerial3.read();
        MySerial1.write(buff123);
        CONNECT_buf[CONNECT_buf_index] = buff123;
        CONNECT_buf_index++;
    }
    connect = String(CONNECT_buf);
    MySerial1.print("connect = ");
    MySerial1.println(connect);

    if (connect.lastIndexOf("FAIL") != -1)
    {
        MySerial1.println("CONNECT TO SERVER FAIL");
        connect_flag = 0;
    }
    else if (connect.lastIndexOf("CONNECT OK") != -1)
    {
        MySerial1.println("CONNECT TO SERVER OK");
        connect_flag = 1;
    }

    return (connect_flag);
}

void SIM868::try_connect_to_server() // выполняем попытку подключиться к серверу
{
    MySerial3.println("ATE0");
    while (MySerial3.available())
        read_SIM868();
    delay(100);

    MySerial3.println("AT+CIPCLOSE"); // закрываем старые TCP соединения
    delay(500);
    read_SIM868();

    MySerial3.println("AT+CSQ");
    read_SIM868();
    delay(100);

    MySerial3.println("AT+CREG?");
    read_SIM868();
    delay(100);

    MySerial3.println("AT+CGATT?");
    read_SIM868();
    delay(100);

    MySerial3.println("AT+CSTT=\"CMNET\"");
    read_SIM868();
    delay(100);

    MySerial3.println("AT+CIICR");
    delay(100);

    MySerial3.println("AT+CIFSR");
    read_SIM868();

    MySerial3.println("AT+CIPSTART=\"TCP\",\"80.74.24.27\",5000");
    delay(3000);
}

void SIM868::setup_gprs_parameter() // настраиваем ппараметры GPRS (APN)
{
    delay(15000);
    read_SIM868();
    MySerial3.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    read_SIM868();
    delay(100);
    MySerial3.println("AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"");
    read_SIM868();
    delay(100);
    MySerial3.println("AT+SAPBR=1,1");
    read_SIM868();
    delay(100);
}

void SIM868::SIM868_GPS_Power_Up() // включаем GPS
{
    MySerial3.write("AT+CGNSPWR=1\n"); // подаем питание на GPS
    delay(100);
    read_SIM868();
}

void SIM868::SIM868_Power_SW(int SIM868_PWR_Pin) // включаем/выключаем Е52
{
    pinMode(SIM868_PWR_Pin, OUTPUT);
    digitalWrite(SIM868_PWR_Pin, HIGH);
    delay(100);
    digitalWrite(SIM868_PWR_Pin, LOW);
    delay(1000);
    digitalWrite(SIM868_PWR_Pin, HIGH);
    delay(3000);
    pinMode(SIM868_PWR_Pin, INPUT);
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
    // MySerial1.print("Get GPS:");
    MySerial1.println("clear_telemetry =");
    
    read_SIM868(); // на всякиий случай, перед получением корд читаем юарт, чтобы буфер был гарантированно пуст
    MySerial3.write("AT+CGNSINF\n");
    
    MySerial1.println("get telemetry =");
    delay(5);
    while (MySerial3.available())
    {
        GPS_str = MySerial3.readString();
        MySerial1.println(GPS_str);
    }
    // GPS_str = "1,1,20240208183233.000,55.643222,37.336658,336.55,0.00,323.0,1,,0.9,1.2,0.8,,12,10,9,,33,,";//подмена для отладки

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

    lontitude = lontitude.substring(0, lontitude.indexOf(".") + 5);
    lattitude = lattitude.substring(0, lattitude.indexOf(".") + 5);
    /*
    MySerial1.println(lattitude);
    MySerial1.println(lattitude.length());
    MySerial1.println(lontitude);
    MySerial1.println(lontitude.length());
    */
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
    String data_transmitt = " " + Module_ADDR + " " + lattitude + " " + lontitude + " " + altitude + " " + altitude_rate + " " + speed + " " + status_count + " " + course; //+ " " + router_hop;

    return (data_transmitt);
}
