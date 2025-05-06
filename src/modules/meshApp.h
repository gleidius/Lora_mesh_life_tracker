#pragma once
#include <Arduino.h>
#include "debug.h"

class MeshApp
{
private:
    HardwareSerial *_streamRef;

public:
    void setup(HardwareSerial *streamObject);
    void sendMessage(String someText);
    int available(void);
    String recieveMessage(void);
};

extern MeshApp meshApp;