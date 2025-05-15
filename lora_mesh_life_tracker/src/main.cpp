#include "functions.h"

void setup() //========================== SETUP ===========================
{ 
  init_board();
}

void loop()
{ // ======================== LOOP ===============================

  unsigned long start_time, timeout_altrate  = millis(); // таймер
  int butt_count = 1;
  int status_count = 1;
  int power_counter = 22;

  bool connect_flag = 0;
/*
  struct Display_coordinates 
  {
    int Power_Xpos, Power_
  };
  */
  
  String Module_ADDR = Set_E52_ADDR();  // устанавливаем и запоминаем адрес Е52

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("    === " + Module_ADDR + " ===");  // !!!!!!!!!!!!! можно попробовать заменить все инты на структуру и потом её return();

  display.print("Power (3), dBm: ");
  int Power_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
  int Power_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
  display.println("22");

  display.print("S/R (4): ");
  int SR_Xpos = display.getCursorX(); // позиция Х курсора при написании Speed/Rate
  int SR_Ypos = display.getCursorY(); // позиция Y курсора при написании Speed/Rate
  display.println("0");

  display.print("Mode (6): ");
  int Mode_Xpos = display.getCursorX(); // позиция Х курсора при написании режима
  int Mode_Ypos = display.getCursorY(); // позиция Y курсора при написании режима
  display.println("Not");

  display.print("Status: ");
  int Stat_Xpos = display.getCursorX(); // позиция Х курсора при написании статуса
  int Stat_Ypos = display.getCursorY(); // позиция Y курсора при написании статуса
  display.println("Ground");

  display.print("ALT_R: ");
  int ALTR_Xpos = display.getCursorX(); // позиция Х курсора при написании статуса
  int ALTR_Ypos = display.getCursorY(); // позиция Y курсора при написании статуса
  display.println("N/A");

  display.display();

  MySerial3.println("AT+CIPCLOSE");
  delay(500);
  read_SIM868();

  while (true)                                               // определение скороподъемности 
  {    
    if (millis() - timeout_altrate >= 1000){     
      timeout_altrate = millis();
      altitude_rate = get_ar_with_filter(ALTR_Xpos, ALTR_Ypos);
    }
    
    if(digitalRead(STM_SW2) == true)                        // режим настроек 
    {
      get_setup_from_ESP();
    }

      if ((millis() - start_time) >= 5000)                  // режим отправки и отправка
    {
      start_time = millis();
      MySerial1.println("===========================================================================================");

      String data_transmitt = get_telemetry(Module_ADDR, status_count, altitude_rate);

      if (digitalRead(STM_SW6) == false)                   // MESH 
      { 
        draw_pos(Mode_Xpos, Mode_Ypos, "Mesh");
        send_to_mesh_E52(data_transmitt); // отправляем пакет (Если что то с модулем: data_transmitt = "GL 6666 56.452051 84.962577 174.967 1.5 190.4 1 2";)
      }

      else if (digitalRead(STM_SW6) == true)              // INTERNET 
      { 
        draw_pos(Mode_Xpos, Mode_Ypos, "Internet");

        if (connect_flag == 0)
        { 
          try_connect_to_server();
          connect_flag = check_connect_to_server();
        }

        if (connect_flag == 1)
        {
          send_to_server_SIM868(data_transmitt);
        }
      }
    }

    if (digitalRead(STM_BTN1) == false)                                    //  SPEED/RANGE 
    { 
      butt_count = Next_SR(butt_count, SR_Xpos, SR_Ypos);
    }

    if (digitalRead(LORA_RST) == false)                                    //  STATUS  
    { 
      status_count = Next_status(status_count, Stat_Xpos, Stat_Ypos);
    }

    if (digitalRead(STM_SW3) == true)                                      //  POWER 
    {  
      power_counter = Next_power(power_counter, Power_Xpos, Power_Ypos);
    }
  }
}