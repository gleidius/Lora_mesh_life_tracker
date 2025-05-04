#include "E52.h"

E52 e52 = E52();

E52::E52() : _serial(1, 2) {}

bool E52::setup()
{
    _serial.begin(115200);
}

// функция изменения параметра скорость/дальность
bool E52::setRs(int rs)
{
    String at = "AT+RATE=" + String(rs);
    DEBUGF("Скорость/Дальность = ", rs);
    return sendCommand(at);
}

// функиця отправки AT-команды в Е52
bool E52::sendCommand(String command)
{
    String response = _serial.sendCommand(command);
    DEBUGLN(response);
    if (response == "No answer")
        return false;
    return true;
}

// void E52::sendToMesh(String data)
// {
//     S_Serial.println(data);
// }

// void E52::setPower(int power)
// { // функция настройки мощности
//     String pw = String(power);
//     String at = "AT+POWER=";
//     String zero = ",0";
//     at.concat(pw);
//     at.concat(zero);
//     S_Serial.println(at);
//     MySerial1.println("Мощность: ");
//     String dbm = " дБм";
//     pw.concat(dbm);
//     MySerial1.println(pw);
// }

// String E52::receiveFromMesh()
// { // функция чтения UART
//     String message = "";
//     while (S_Serial.available())
//     {
//         message = S_Serial.readStringUntil('\n');
//     }
//     return message;
// }