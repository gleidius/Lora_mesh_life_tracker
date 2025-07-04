#include "e52.h"

e52::e52(HardwareSerial &Serial1, HardwareSerial &Serial3)
    : S_Serial(Serial3), MySerial1(Serial1) {}

void e52::send_command(String command) // функиця отправки AT-команды в Е52
{
    S_Serial.println(command);
    delay(100);
    while (S_Serial.available())
    {
        byte buff123 = S_Serial.read();
        MySerial1.write(buff123);
    }
}

void e52::set_power(int power) // функция настройки мощности
{
    String pw = String(power);
    String at = "AT+POWER=";
    String zero = ",0";
    at.concat(pw);
    at.concat(zero);
    S_Serial.println(at);
    // MySerial1.print("Мощность: ");
    String dbm = " дБм";
    pw.concat(dbm);
    // MySerial1.println(pw);
}

void e52::read_SSerial() // функция чтения Soft UART с задержкой
{
    delay(100);
    while (S_Serial.available())
    {
        byte buff123 = S_Serial.read();
        MySerial1.write(buff123);
    }
}

int e52::set_pause(int pause) // функция установки паузы передачи
{
    MySerial1.print("Пауза, мс: ");
    MySerial1.println(pause);
    int test_delay = pause - 300;
    return (test_delay);
}

void e52::set_rs(int rs) // функция изменения параметра скорость/дальность
{
    String range_speed = String(rs);
    String at = "AT+RATE=";
    at.concat(range_speed);
    S_Serial.println(at);
    MySerial1.print("Скорость/Дальность = ");
    MySerial1.println(range_speed);
    read_SSerial();
}

void e52::set_SRC_ADDR(int SRC) // функция изменения собственного адреса
{
    String range_speed = String(SRC);
    String at = "AT+SRC_ADDR=";
    String save = ",1";
    at.concat(range_speed);
    at.concat(save);
    S_Serial.println(at);
    MySerial1.print("Собственный адрес = ");
    MySerial1.println(range_speed);

    while (S_Serial.available())
    {
        byte buff123 = S_Serial.read();
        MySerial1.write(buff123);
    }
}

String e52::Set_E52_ADDR() // устанавливаем адрес Е52 по последним 4-м ицфрам МАС адреса
{
    // read_SSerial();
    send_command("AT+SRC_ADDR=1234,1");
    char MAC_buff[50] = "1010";
    int MAC_buff_index = 0;

    S_Serial.print("AT+MAC=?");
    MySerial1.print("AT+MAC=?");
    delay(100);

    while (S_Serial.available())
    {
        byte buff123 = S_Serial.read();
        MySerial1.write(buff123);
        MAC_buff[MAC_buff_index] = buff123;
        MAC_buff_index++;
    }
    String MAC_addr = String(MAC_buff);
    String Module_ADDR = MAC_addr.substring(MAC_addr.indexOf(",") + 1);
    Module_ADDR = Module_ADDR.substring(Module_ADDR.length() - 6, Module_ADDR.length() - 2);

    MySerial1.print("Module_addr=");
    MySerial1.print(Module_ADDR);
    send_command("AT+SRC_ADDR=" + Module_ADDR + ",1");

    return (Module_ADDR);
}

void e52::send_to_mesh_E52(String data_transmitt) // отправляем данные в меш при помщи Е52
{
    S_Serial.println(data_transmitt);
    MySerial1.print("pack = ");
    MySerial1.println(data_transmitt);
}

void e52::E52_default_init() // инициализируемся по дефолту
{
    send_command("AT+POWER=22,0");     // устанавливаем базовую мощность
    send_command("AT+DST_ADDR=404,0"); // задаем целевой адрес
    send_command("AT+OPTION=1,0");     // задаем режим передачи (1 - unicast (одноадресная))
    send_command("AT+RATE=0");         // устанавливаем параметр скорость/дальность
}

String e52::read_router_hop() // считываем номер модуля через который произошел hop
{
    S_Serial.print("AT+ROUTER_READ=?");
    delay(5);
    while (S_Serial.available())
    {
        String router_table = S_Serial.readString();
        // MySerial1.println(router_table);
        if (router_table.indexOf("404") != -1)
        {

            router_table = router_table.substring(router_table.indexOf("404"));
            // MySerial1.println(router_table);
            // MySerial1.println("404 detected");

            String router_hop = router_table.substring(3);
            // MySerial1.print("hop =");
            // MySerial1.println(router_hop);
            while (router_hop.startsWith(" ") == true)
            {
                router_hop = router_hop.substring(1);
                // MySerial1.println(router_hop);
            }
            router_hop = router_hop.substring(0, 4);
            // MySerial1.println(router_hop);
            // MySerial1.println(router_hop.length());
            if (router_hop.endsWith(" ") == 1)
            {
                router_hop = router_hop.substring(0, 3);
            }

            return (router_hop);
        }
        else
        {
            return ("E");
        }
    }
}
