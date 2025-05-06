#include "meshApp.h"

MeshApp meshApp;

void MeshApp::setup(HardwareSerial *streamObject)
{
    _streamRef = streamObject;
}

void MeshApp::sendMessage(String someText)
{
    _streamRef->println(someText);
}

int MeshApp::available(void)
{
    return _streamRef->available();
}

String MeshApp::recieveMessage(void)
{
    String response;
    while (_streamRef->available())
    {
        response = _streamRef->readStringUntil('\n');
        DEBUGLN(response);
    }
    return response;
}