#include "functions.h"

void setup() //========================== SETUP ===========================
{ MySerial1.println("========= START =========");
  init_board();
  MySerial1.println("========= STOP SETUP =========");
}

void loop()
{ // ======================== LOOP ===============================

  unsigned long start_time, timeout_altrate  = millis();     // таймер
  int butt_count = 1;
  int status_count = 1;
  int power_counter = 22;

  bool connect_flag = 0;
  
  String Module_ADDR = Set_E52_ADDR();                       // устанавливаем и запоминаем адрес Е52
  Display_coordinates cord = init_menu(Module_ADDR);

  MySerial3.println("AT+CIPCLOSE");                          // закрываем старые TCP соединения 
  delay(500);
  read_SIM868();

  while (true)                                               
  {    
    if (millis() - timeout_altrate >= 1000){                // определение скороподъемности 
      timeout_altrate = millis();
      altitude_rate = get_ar_with_filter(cord.ALTR_Xpos, cord.ALTR_Ypos);
    }
    
    if(digitalRead(STM_SW2) == true)                        // режим настроек 
    {
      get_setup_from_ESP();                                 // получаем строку с настройками инастраиваемся
    }

      if ((millis() - start_time) >= 5000)                  // режим отправки и отправка
    {
      start_time = millis();
      MySerial1.println("===========================================================================================");

      String data_transmitt = get_telemetry(Module_ADDR, status_count, altitude_rate);

      if (digitalRead(STM_SW6) == false)                         // MESH 
      { 
        draw_pos(cord.Mode_Xpos, cord.Mode_Ypos, "Mesh");
        send_to_mesh_E52(data_transmitt); // отправляем пакет (Если что то с модулем: data_transmitt = "GL 6666 56.452051 84.962577 174.967 1.5 190.4 1 2";)
      }

      else if (digitalRead(STM_SW6) == true)                    // INTERNET 
      { 
        draw_pos(cord.Mode_Xpos, cord.Mode_Ypos, "Internet");

        if (connect_flag == 0)
        { 
          try_connect_to_server();                            // пытаемя подключиьтся к серверу
          connect_flag = check_connect_to_server();           // проверяем получилось ли подключиться 
        }

        if (connect_flag == 1)
        {
          send_to_server_SIM868(data_transmitt);              // если получилось подключиться то отправляем данные
        }
      }
    }

    if (digitalRead(STM_BTN1) == false)                                                 //  SPEED/RANGE 
    { 
      butt_count = Next_SR(butt_count, cord.SR_Xpos, cord.SR_Ypos);
    }

    if (digitalRead(LORA_RST) == false)                                                 //  STATUS  
    { 
      status_count = Next_status(status_count, cord.Stat_Xpos, cord.Stat_Ypos);
    }

    if (digitalRead(STM_SW3) == true)                                                   //  POWER 
    {  
      power_counter = Next_power(power_counter, cord.Power_Xpos, cord.Power_Ypos);
    }
  }
}
