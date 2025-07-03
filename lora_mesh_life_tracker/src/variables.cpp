#include "variables.h"
#include "serial_init.h"

// #include "functions.h"

String APN, power, rate, dst_addr, param5;

// float Preshure[2]{101325, 101325};
// float alt_rate_massiv[5]{0, 0, 0, 0, 0};
// int time_massiv[2]{0, 0};
String altitude_rate = "E";
// unsigned long alt_rate_time = millis();

unsigned long start_time, timeout_altrate, SW_tiomeout_time = millis(); // таймер
int butt_count = 1;
int status_count = 1;
int power_counter = 22;
int TX_timeout_random_piece = 0;
int counter_TX_pack = 0;
bool connect_flag = 0;
// int random_piece_upper_limit = 0;
String data_transmitt = "";
String data_transmitt_old = "";
String router_hop = "E";
String Module_ADDR = "1234"; // устанавливаем и запоминаем адрес Е52
int state_SW1, state_SW2, state_SW3, state_SW4, state_SW5, state_SW6 = 0;
int old_state_SW1, old_state_SW2, old_state_SW3, old_state_SW4;
