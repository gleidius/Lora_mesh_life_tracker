#pragma once
#include "configuration.h"

class E52
{
public:
    E52();

    bool setup();
    // функция изменения параметра скорость/дальность
    bool setRs(int rs);
    uint8_t getRs();

    void setPower(int power);
    int8_t getPower();

    void setDstAddr();
    uint32_t getSrcAddr();

    bool sendCommand(String command);

    void sendToMesh(String data);

    String receiveFromMesh();

private:
    SerialHandler _serial;
};

extern E52 e52;
