#include "variables.h"
#include "serial_init.h"

// #include "functions.h"

String APN, power, rate, dst_addr, param5;
String altitude_rate = "E";

unsigned long start_time, timeout_altrate, SW_tiomeout_time = millis(); // таймер
int butt_count = 1;
int status_count = 1;
int power_counter = 22;
int TX_timeout_random_piece = 0;
// int counter_TX_pack = 0;
// bool connect_flag = 0;
// String data_transmitt = "";
// String data_transmitt_old = "";
String router_hop = "E";

String module_ADDR = "2222"; // устанавливаем и запоминаем адрес Е52

int state_SW1, state_SW2, state_SW3, state_SW4, state_SW5, state_SW6 = 0;
int old_state_SW1, old_state_SW2, old_state_SW3, old_state_SW4;
