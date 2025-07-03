#pragma once
#include <Arduino.h>

// On-Board LEDs
extern uint8_t LED_PC13;

// LoRa LEDs
extern uint8_t LED_LED1;
extern uint8_t LED_LED2;

// On-Board buttons                                                               // КНОПОЧКИ
extern uint8_t STM_LT;
extern uint8_t STM_DN;
extern uint8_t STM_OK;
extern uint8_t STM_RT;
extern uint8_t STM_UP;

// uint8_t STM_SW1 = PC13;                                                        // ПЕРЕКЛЮЧАТЕЛИ
extern uint8_t STM_SW1;
extern uint8_t STM_SW2;
extern uint8_t STM_SW3;
extern uint8_t STM_SW4;
extern uint8_t STM_SW5;
extern uint8_t STM_SW6;

// SIM868 GPIO
extern uint8_t SIM_SLEEP;
extern uint8_t SIM_PWRK;

void init_pinout();
