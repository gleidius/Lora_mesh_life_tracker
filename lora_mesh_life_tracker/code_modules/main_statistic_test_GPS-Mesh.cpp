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
  {    
    if (millis() - timeout_altrate >= 1000){                // определение скороподъемности 
      timeout_altrate = millis();
      altitude_rate = get_ar_with_filter(cord.ALTR_Xpos, cord.ALTR_Ypos);
    }
    /*
    if(digitalRead(STM_SW2) == false)                        // SETTINGS
    {
      get_setup_from_ESP();                                 // получаем строку с настройками инастраиваемся
    }*/

      if ((millis() - start_time) >= 1000)                  // режим отправки и отправка
    {
      start_time = millis();
      /*
      MySerial1.print("TX_timeout=");
      MySerial1.println(10000+TX_timeout_random_piece);
      */
      //TX_timeout_random_piece = random(0,random_piece_upper_limit);
      /*
      MySerial1.print("Random= ");
      MySerial1.println(random_piece_upper_limit);
      */
      //MySerial1.println("===========================================================================================");

      String data_transmitt = get_telemetry(Module_ADDR, status_count, altitude_rate);

      if (digitalRead(STM_SW6) == false)                         // MESH 
      { 
        draw_pos(cord.Mode_Xpos, cord.Mode_Ypos, "Mesh");
        send_to_mesh_E52("GL" + data_transmitt); // отправляем пакет (Если что то с модулем: data_transmitt = "GL 6666 56.452051 84.962577 174.967 1.5 190.4 1 2";)
      }
    }


  
    state_SW1 = !digitalRead(STM_SW1);
    state_SW2 = !digitalRead(STM_SW2);
    state_SW3 = !digitalRead(STM_SW3);
    state_SW4 = !digitalRead(STM_SW4);
    state_SW5 = !digitalRead(STM_SW5);
    state_SW6 = !digitalRead(STM_SW6);
    /*
    MySerial1.print("SW1= ");
    MySerial1.println(state_SW1);
    MySerial1.print("SW2= ");
    MySerial1.println(state_SW2);
    MySerial1.print("SW3= ");
    MySerial1.println(state_SW3);
    MySerial1.print("SW4= ");
    MySerial1.println(state_SW4);
    MySerial1.print("SW5= ");
    MySerial1.println(state_SW5);
    MySerial1.print("SW6= ");
    MySerial1.println(state_SW6);*/

if ((state_SW1 != old_state_SW1)or(state_SW2 != old_state_SW2)){
  if (state_SW1+state_SW2>1)                                                 //  SPEED/RANGE 
  { 
    draw_pos(cord.SR_Xpos,cord.SR_Ypos,"ERROR");
  }
  else if(state_SW1 == 1){
    set_rs(1);
    draw_pos(cord.SR_Xpos,cord.SR_Ypos,"1");
  }
  else if(state_SW2 == 1){
    set_rs(2);
    draw_pos(cord.SR_Xpos,cord.SR_Ypos,"2");
  }
  else if((state_SW1 == 0)and(state_SW2 == 0)){
    set_rs(0);
    draw_pos(cord.SR_Xpos,cord.SR_Ypos,"0");
  }
  old_state_SW1=state_SW1;
  old_state_SW2=state_SW2;
}
  
if ((state_SW3 != old_state_SW3)or(state_SW4 != old_state_SW4)){
  if(state_SW3+state_SW4>1){
    draw_pos(cord.pause_Xpos,cord.pause_Ypos,"ERROR");
  }
  else if(state_SW3 == 1){
    random_piece_upper_limit=7000;
        draw_pos(cord.pause_Xpos,cord.pause_Ypos,"3-10");
     }
     else if(state_SW4 == 1){
        random_piece_upper_limit=12000;
        draw_pos(cord.pause_Xpos,cord.pause_Ypos,"3-15");
     }
     else if((state_SW3 == 0)and(state_SW4 == 0)){
        random_piece_upper_limit=2000;
        draw_pos(cord.pause_Xpos,cord.pause_Ypos,"3-5");
     }
    old_state_SW3=state_SW3;
    old_state_SW4=state_SW4;
    }
/*
  if(state_SW5 == 1){
    set_power(-8);
    draw_pos(cord.Power_Xpos,cord.Power_Ypos,"-8");
  }
  else{
    set_power(22);
    draw_pos(cord.Power_Xpos,cord.Power_Ypos,"22");
  }*/
  }
  }

