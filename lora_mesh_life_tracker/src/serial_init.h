#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

// Serial UART
extern uint8_t UART1_TX;
extern uint8_t UART1_RX;
extern HardwareSerial MySerial1;

// Sim UART
extern uint8_t UART3_TX;
extern uint8_t UART3_RX;
extern HardwareSerial MySerial3;

// LoRa URAT
extern uint8_t UART2_TX;
extern uint8_t UART2_RX;
extern HardwareSerial S_Serial;