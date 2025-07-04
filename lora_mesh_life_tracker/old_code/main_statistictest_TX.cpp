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
  int counter_TX = 0;
  int pause_counter = 0;
  int butt_count = 1;
  int power_counter = 22;
  int pause = 600;
  unsigned long timer = millis();

  while (true)                                               
  {    
    if (digitalRead(STM_LT) == false)                                                 //  SPEED/RANGE 
    { 
      butt_count = Next_SR(butt_count, cord.SR_Xpos, cord.SR_Ypos);
    }
        if (digitalRead(STM_DN) == false)                                                   //  POWER 
    {  
      power_counter = Next_power(power_counter, cord.Power_Xpos, cord.Power_Ypos);
    }

    if (digitalRead(STM_RT) == false)                                                 //  PAUSE
    { 
      if(pause_counter == 1){
        pause = 600;
            draw_pos(cord.pause_Xpos,cord.pause_Ypos,"600");
      }
      if(pause_counter == 2){
        pause = 1200;
            draw_pos(cord.pause_Xpos,cord.pause_Ypos,"1200");
      }
      if(pause_counter == 3){
         pause = 2000;
            draw_pos(cord.pause_Xpos,cord.pause_Ypos,"2000");
      }
      if(pause_counter == 4){
        pause = 3000;
            draw_pos(cord.pause_Xpos,cord.pause_Ypos,"3000");
      }
      if(pause_counter == 5){
        pause = 5000;
            draw_pos(cord.pause_Xpos,cord.pause_Ypos,"5000");
      }
      if(pause_counter == 6){
        pause = 10000;
            draw_pos(cord.pause_Xpos,cord.pause_Ypos,"10000");
            pause_counter=0;
      }
        pause_counter++;
      delay(200);
    }
    if(digitalRead(STM_OK) == false){
      while(true){

        if(millis() - timer >= pause){
          timer = millis();
          S_Serial.print("GL 6520 56.451878 84.962554 186.780 -0.00 2.91 1 103.1");
          counter_TX++;
          MySerial1.println(counter_TX);
          draw_pos(0,56,String(counter_TX));
        }

        if(digitalRead(STM_RT) == false){
      S_Serial.print("AT+ROUTER_READ=?");
      delay(5);
      while (S_Serial.available())
      {
        byte buff123 = S_Serial.read();
        MySerial1.write(buff123);
      }
      //delay(200);
    }
        
        if(millis() >= 1000*60*10){
          while(true){
          }
        }
      }
    }
  }
}
