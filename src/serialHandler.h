#include <Arduino.h>

class SerialHandler
{
public:
    SerialHandler(uint32_t rx, uint32_t tx);

    void begin(unsigned long baud);
    bool available(void);

    String sendCommand(String command);
    void sendMessage(String message);
    String receiveMessage(void);

private:
    HardwareSerial _serial = nullptr;
};