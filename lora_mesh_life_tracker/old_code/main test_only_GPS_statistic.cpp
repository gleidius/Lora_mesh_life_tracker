#include "functions.h"

void setup() 
{//========================== SETUP ===========================
  
  MySerial1.println("========= START =========");
  init_board();
  MySerial1.println("========= STOP SETUP =========");
}

void loop()
{ // ======================== LOOP ===============================

  unsigned long start_time, timeout_altrate, SW_tiomeout_time  = millis();     // таймер
  int butt_count = 1;
  int status_count = 1;
  int power_counter = 22;
  int TX_timeout_random_piece = 0;
  bool connect_flag = 0;
  int random_piece_upper_limit = 0;
  String Module_ADDR = Set_E52_ADDR();                       // устанавливаем и запоминаем адрес Е52
  Display_coordinates cord = init_menu(Module_ADDR);

  int state_SW1,state_SW2,state_SW3,state_SW4,state_SW5,state_SW6 = 0;
  int old_state_SW1,old_state_SW2,old_state_SW3,old_state_SW4 = 0;
  while (true)                                               
  {       if ((millis() - start_time) >= 1000)                  // режим отправки и отправка
    {start_time=millis();
       String GPS_str = "ERROR";

          MySerial1.print("Get GPS:");
      read_SIM868();                              // на всякиий случай, перед получением корд читаем юарт, чтобы буфер был гарантированно пуст
      MySerial3.write("AT+CGNSINF\n");
      delay(5);
      while (MySerial3.available())
      {
        GPS_str = MySerial3.readString();
        MySerial1.println(GPS_str);
      }
  }
  }
}