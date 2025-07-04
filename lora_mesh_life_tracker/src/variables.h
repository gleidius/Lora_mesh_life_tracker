#pragma once
#include <Arduino.h>

extern String altitude_rate;

struct Display_coordinates
{
    int Power_Xpos, Power_Ypos;
    int SR_Xpos, SR_Ypos;
    int Mode_Xpos, Mode_Ypos;
    int Stat_Xpos, Stat_Ypos;
    int ALTR_Xpos, ALTR_Ypos;
    int pause_Xpos, pause_Ypos;
};

extern unsigned long start_time, timeout_altrate, SW_tiomeout_time; // таймер
extern int butt_count;
extern int status_count;
extern int power_counter;
extern int TX_timeout_random_piece;
// extern int counter_TX_pack;
// extern bool connect_flag;
extern String module_ADDR; // устанавливаем и запоминаем адрес Е52
// extern String data_transmitt;
// extern String data_transmitt_old;
extern String router_hop;
extern Display_coordinates cord;
extern int state_SW1, state_SW2, state_SW3, state_SW4, state_SW5, state_SW6;
extern int old_state_SW1, old_state_SW2, old_state_SW3, old_state_SW4;