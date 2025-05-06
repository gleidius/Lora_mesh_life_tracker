#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <serialEEPROM.h>

extern serialEEPROM myEEPROM;

extern uint8_t UART1_TX;
extern uint8_t UART1_RX;
extern HardwareSerial MySerial1;

// On-Board LEDs
extern uint8_t LED_PC13;

// LoRa LEDs
extern uint8_t LED_LED1;
extern uint8_t LED_LED2;

// SIM868 UART
extern uint8_t UART3_TX;
extern uint8_t UART3_RX;
extern HardwareSerial MySerial3;

// On-Board buttons                                                               // КНОПОЧКИ
extern uint8_t STM_BTN1;
extern uint8_t LORA_PA0;
extern uint8_t LORA_RST;

// uint8_t STM_SW1 = PC13;                                                        // ПЕРЕКЛЮЧАТЕЛИ
extern uint8_t STM_SW2;
extern uint8_t STM_SW3;
extern uint8_t STM_SW4;
extern uint8_t STM_SW5;
extern uint8_t STM_SW6;

// SIM868 GPIO
extern uint8_t SIM_SLEEP;
extern uint8_t SIM_PWRK;

extern uint8_t switches[];

extern uint8_t UART2_TX;
extern uint8_t UART2_RX;
extern HardwareSerial S_Serial;

void read_SIM868(); // функция чтения ответа от SIM868

void send_to_server_SIM868(String dataTransmit); // отправляем данные на сервер используя SIM868

bool check_connect_to_server(); // функция проверки соединения с сервером

void try_connect_to_server(); // выполняем попытку подключиться к серверу

int Next_status(int status_count, int Stat_Xpos, int Stat_Ypos); // выполняем смену статуса

void SIM868_GPS_Power_Up(); // включаем GPS

void SIM868_Power_SW(int SIM868_PWR_Pin); // включаем/выключаем Е52

String get_telemetry(String Module_ADDR, int status_count); // получаем телеметрию
