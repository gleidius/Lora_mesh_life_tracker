#include "pinout.h"

// On-Board LEDs
uint8_t LED_PC13 = PC13;

// LoRa LEDs
uint8_t LED_LED1 = PB8;
uint8_t LED_LED2 = PB9;

// On-Board buttons                                                               // КНОПОЧКИ
uint8_t STM_LT = PB3;
uint8_t STM_DN = PA15;
uint8_t STM_OK = PA12;
uint8_t STM_RT = PA11;
uint8_t STM_UP = PA8;

// On-Board switches
uint8_t STM_SW1 = PC14;
uint8_t STM_SW2 = PC15;
uint8_t STM_SW3 = PA0;
uint8_t STM_SW4 = PA1;
uint8_t STM_SW5 = PB0;
uint8_t STM_SW6 = PB1;

// #define STM_SW6 PB1

uint8_t switches[] = {
    STM_SW1,
    STM_SW2,
    STM_SW3,
    STM_SW4,
    STM_SW5,
    STM_SW6};

// SIM868 GPIO
uint8_t SIM_SLEEP = PA4;
uint8_t SIM_PWRK = PB15;

void init_pinout() // инициализируем пины и настройки экранчика !!!достать отсюда пины
{
    pinMode(STM_LT, INPUT); // инициализируем кнопочки
    pinMode(STM_DN, INPUT);
    pinMode(STM_OK, INPUT);
    pinMode(STM_RT, INPUT);
    pinMode(STM_UP, INPUT);

    pinMode(STM_SW1, INPUT_PULLUP);
    pinMode(STM_SW2, INPUT_PULLUP);
    pinMode(STM_SW3, INPUT_PULLUP);
    pinMode(STM_SW4, INPUT_PULLUP);
    pinMode(STM_SW5, INPUT_PULLUP);
    pinMode(STM_SW6, INPUT_PULLUP);

    // инициализируем  пины SIM868
    pinMode(SIM_SLEEP, INPUT);
    pinMode(SIM_PWRK, INPUT);
}
