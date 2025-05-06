#include "e52.h"

E52 e52;

bool E52::setup(HardwareSerial *streamObject)
{
    bool flag;
    _streamRef = streamObject;
    setSrcAddr(getMacAddrLora());
    flag += !sendCommand("AT+POWER=14,0");     // устанавливаем базовую мощность
    flag += !sendCommand("AT+DST_ADDR=404,0"); // задаем целевой адрес
    flag += !sendCommand("AT+OPTION=1,0");     // задаем режим передачи (1 - unicast (одноадресная))
    flag += !setRs(0);
    return !flag;
}

// Установка скорости/дальности
bool E52::setRs(uint8_t rs)
{
    String at = "AT+RATE=" + String(rs);
    return sendCommand(at);
}

// Отправка AT-команд в E52
bool E52::sendCommand(String command)
{
    String response;
    DEBUGLN(command);
    _streamRef->print(command);
    delay(100);
    while (_streamRef->available())
    {
        response = _streamRef->readStringUntil('\n');
        DEBUGLN(response);
    }
    if (response.indexOf("CMD_ERR") != -1)
        return false;
    return true;
}

// Отправка сообщений в mesh
void E52::sendMessage(String message)
{
    _streamRef->println(message);
    DEBUG("Message to Mesh: ");
    DEBUGLN(message);
}

// Установка мощности передачи
void E52::setPower(int8_t power)
{
    if (power > 22)
        power = 22;
    if (power < -8)
        power = -8;
    String command = "AT+POWER=";
    command += String(power);
    command += ",0";
    sendCommand(command);
}

// Установка собственного адреса
void E52::setSrcAddr(uint16_t srcAddr)
{
    srcAddr = srcAddr;
    String command = "AT+SRC_ADDR=";
    command += String(srcAddr);
    command = ",1";
    sendCommand(command);
}

uint16_t E52::getSrcAddr(void)
{
    return srcAddr;
}

uint16_t E52::getMacAddrLora()
{
    String input;
    _streamRef->print("AT+MAC=?");
    DEBUGLN(5);
    delay(100);
    while (int i = _streamRef->available())
    {
        DEBUGLN(i);
        input = _streamRef->readString();
    }
    DEBUGLN(input);
    // Находим позицию начала шестнадцатеричного числа
    int pos = input.indexOf("0x");
    if (pos != -1)
    {
        // Извлекаем шестнадцатеричное число
        String hexNumber = input.substring(pos + 2, pos + 10); // 8 символов после "0x"

        // Берем последние 4 символа
        String lastFourHex = hexNumber.substring(hexNumber.length() - 4);

        // Преобразуем шестнадцатеричное число в десятичное
        unsigned long decimalValue = strtoul(lastFourHex.c_str(), NULL, 16);
        return decimalValue;
    }
    else
        return millis() % 65536;
}

int E52::available(void)
{
    return _streamRef->available();
}

String E52::recieveMessage(void)
{
    String response;
    while (_streamRef->available())
    {
        response = _streamRef->readStringUntil('\n');
        DEBUGLN(response);
    }
    return response;
}