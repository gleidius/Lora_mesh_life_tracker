#include "serial_init.h"

// Serial UART
uint8_t UART1_TX = PA9;
uint8_t UART1_RX = PA10;
HardwareSerial Terminal_UART(UART1_RX, UART1_TX);

// Sim UART
uint8_t UART3_TX = PB10;
uint8_t UART3_RX = PB11;
HardwareSerial SIM868_UART(UART3_RX, UART3_TX);

// LoRa URAT
uint8_t UART2_TX = PA2;
uint8_t UART2_RX = PA3;
HardwareSerial S_Serial(UART2_RX, UART2_TX);