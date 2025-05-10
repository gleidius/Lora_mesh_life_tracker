#include "e52.h"

E52 e52;

void E52::setup(HardwareSerial *streamObject)
{
    _streamRef = streamObject;
    getParametersOfE52();
    setSrcAddr(804);
}

// Установка скорости/дальности
void E52::setRs(uint8_t rs)
{
    String at = "AT+RATE=" + String(rs);
    sendCommand(at);
}

// Установка режима передачи
// 1 - unicast (одноадресная)
void E52::setOption(uint8_t option)
{
    String at = "AT+OPTION=" + String(option);
    sendCommand(at);
}

// Установка целевого адреса
void E52::setDstAddr(uint16_t DstAddr)
{
    String at = "DST_ADDR=" + String(DstAddr);
    sendCommand(at);
}

// Отправка AT-команд в E52
String E52::sendCommand(String command, const char *separation)
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
        return "error";
    if (separation != nullptr)
        return response.substring(response.indexOf(separation) + 1);
    return response;
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
    command += ",1";
    sendCommand(command);
}

// Установка собственного адреса
void E52::setSrcAddr(uint16_t srcAddr)
{
    this->srcAddr = srcAddr;
    String command = "AT+SRC_ADDR=";
    command += String(srcAddr);
    command += ",1";
    sendCommand(command);
}

uint16_t E52::getSrcAddr(void)
{
    return srcAddr;
}

uint16_t E52::getMacAddrLora(void)
{
    String input;
    input = sendCommand("AT+MAC=?", "=");
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

        DEBUG("MAC addres Lora: ");
        DEBUGLN(decimalValue);

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

void E52::setDefaultParameters(void)
{
    setSrcAddr(getMacAddrLora());
    setPower(E52_POWER);
    setDstAddr(E52_DST_ADDRESS);
    setOption(E52_OPTION);
    setRs(E52_RATE);
}

void E52::getParametersOfE52(void)
{
    DEBUGLN("Parameters E52 from memory");
    power = sendCommand("AT+POWER=?", ",").toInt();
    option = sendCommand("AT+OPTION=?", ",").toInt();
    dstAddr = sendCommand("AT+DST_ADDR=?", ",").toInt();
    srcAddr = sendCommand("AT+SRC_ADDR=?", ",").toInt();
    rate = sendCommand("AT+RATE=?", ",").toInt();
}