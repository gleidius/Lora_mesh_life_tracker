#pragma once
#include <Arduino.h>
#include "configuration.h"

class E52
{
private:
    HardwareSerial *_streamRef;

    int8_t power;
    uint8_t rate;
    uint8_t option;
    uint16_t srcAddr;
    uint16_t dstAddr;

public:
    void setup(HardwareSerial *streamObject);

    String sendCommand(String command, const char *separation = nullptr);
    void sendMessage(String data);
    int available(void);
    String recieveMessage(void);

    void setRs(uint8_t rs);
    uint8_t getRs(void);

    void setPower(int8_t power);
    int8_t getPower(void);

    void setOption(uint8_t option);

    void setDstAddr(uint16_t DstAddr);

    void setSrcAddr(uint16_t srcAddr);
    uint16_t getSrcAddr(void);

    uint16_t getMacAddrLora(void);

    void setDefaultParameters(void);

private:
    void getParametersOfE52(void);
};

extern E52 e52;
