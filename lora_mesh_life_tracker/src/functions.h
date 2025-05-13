#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <serialEEPROM.h>
#include <GyverBME280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

extern Adafruit_SSD1306 display;//(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
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

extern float Preshure[];
extern float alt_rate_massiv[];
extern int time_massiv[];
extern String altitude_rate;
extern unsigned long alt_rate_time;

void send_command(String command);

void set_power(int power);

void read_SSerial();

int set_pause(int pause);

void set_rs(int rs);

void set_SRC_ADDR(int SRC);

void read_SIM868(); // функция чтения ответа от SIM868

void send_SIM868(String command);        // отправка АТ команды в sim

void draw_pos(int x_pos, int y_pos, String text);// функция отрисовки по позиции, закрашивая строку

void send_to_server_SIM868(String dataTransmit);  // отправляем данные на сервер используя SIM868

bool check_connect_to_server(); // функция проверки соединения с сервером

void try_connect_to_server();  // выполняем попытку подключиться к серверу

void setup_gprs_parameter(); // настраиваем ппараметры GPRS (APN)

int Next_status(int status_count, int Stat_Xpos, int Stat_Ypos); // выполняем смену статуса

int Next_SR(int butt_count, int SR_Xpos, int SR_Ypos); // меняем параметр скорость/дальность

String Set_E52_ADDR(); // устанавливаем адрес Е52 по последним 4-м ицфрам МАС адреса

void send_to_mesh_E52(String data_transmitt); // отправляем данные в меш при помщи Е52

void E52_default_init();  // инициализируемся по дефолту

void SIM868_GPS_Power_Up();    // включаем GPS

void SIM868_Power_SW(int SIM868_PWR_Pin); // включаем/выключаем Е52

int Next_power(int power_counter, int Power_Xpos, int Power_Ypos); // переключаем мощность Е52

String get_telemetry(String Module_ADDR, int status_count, String altitude_rate);    // получаем телеметрию

float get_altitude_rate(float P, float P_pred, int t, int t_pred);

void setup_bmp();

void get_setup_from_ESP();     // получение настроек по меш от ESP

String get_ar_with_filter(int ALTR_Xpos, int ALTR_Ypos);       // получаем и фильтруем скороподъемность