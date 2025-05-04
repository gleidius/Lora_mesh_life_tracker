#include "serialHandler.h"

SerialHandler::SerialHandler(uint32_t rx, uint32_t tx)
{
    HardwareSerial _serial(rx, tx);
}

void SerialHandler::begin(unsigned long baud)
{
    _serial.begin(baud);
}

bool SerialHandler::available(void)
{
    return _serial.available();
}

String SerialHandler::sendCommand(String command)
{
    _serial.println(command);
    delay(100);
    if (_serial.available())
        return _serial.readStringUntil('\n');
}

void SerialHandler::sendMessage(String message)
{
    _serial.println(message);
}

String SerialHandler::receiveMessage(void)
{
    return _serial.readStringUntil('\n');
}