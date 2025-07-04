#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>

// Serial UART
extern uint8_t UART1_TX;
extern uint8_t UART1_RX;
extern HardwareSerial Terminal_UART;

// Sim UART
extern uint8_t UART3_TX;
extern uint8_t UART3_RX;
extern HardwareSerial SIM868_UART;

// LoRa URAT
extern uint8_t UART2_TX;
extern uint8_t UART2_RX;
extern HardwareSerial S_Serial;