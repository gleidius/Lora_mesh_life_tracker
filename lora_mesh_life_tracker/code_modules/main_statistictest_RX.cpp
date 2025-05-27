#include "functions.h"

void setup() 
{//========================== SETUP ===========================
  
  MySerial1.println("========= START =========");
  init_pinout_and_display();
  E52_default_init(); // инициализируем Е52 по дефолту
  MySerial1.println("========= STOP SETUP =========");
}

void loop()
{ // ======================== LOOP ===============================
  String Module_ADDR = Set_E52_ADDR();                       // устанавливаем и запоминаем адрес Е52
  Display_coordinates cord = init_menu(Module_ADDR);
  int counter_RX = 0;
  int power_counter = 22;
  int butt_count = 1;
  unsigned long timer = millis();
  send_command("AT+SRC_ADDR=404,1");
  while (true)                                               
  {    
    if(S_Serial.available()){
      counter_RX++;
      MySerial1.println();
      MySerial1.print(S_Serial.readString());
      MySerial1.print("Num_Pack=");
      MySerial1.println(counter_RX);
      draw_pos(0,56,String(counter_RX));
    }

    if(digitalRead(STM_OK) == false){
      S_Serial.print("AT+ROUTER_READ=?");
      delay(5);
      while (S_Serial.available())
      {
        byte buff123 = S_Serial.read();
        MySerial1.write(buff123);
      }
      //delay(200);
    }
    

    if (digitalRead(STM_LT) == false)                                                 //  SPEED/RANGE 
    { 
      butt_count = Next_SR(butt_count, cord.SR_Xpos, cord.SR_Ypos);
    }
        if (digitalRead(STM_DN) == false)                                                   //  POWER 
    {  
      power_counter = Next_power(power_counter, cord.Power_Xpos, cord.Power_Ypos);
    }
  }
}
