#pragma once
#include <Arduino.h>
#include "debug.h"

class E52
{
private:
    HardwareSerial *_streamRef;
    uint16_t srcAddr = 0;

public:
    bool setup(HardwareSerial *streamObject);

    bool sendCommand(String command);
    void sendMessage(String data);
    int available(void);
    String recieveMessage(void);

    bool setRs(uint8_t rs);
    uint8_t getRs();

    void setPower(int8_t power);
    int8_t getPower();

    void setSrcAddr(uint16_t srcAddr);
    uint16_t getSrcAddr(void);

    uint16_t getMacAddrLora();
};

extern E52 e52;
