#include "functions.h"

void setup()
{ //========================== SETUP ===========================
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) // инициализация дисплея    !!!!! спросить у Александра почему так !!!!!
  {
    MySerial1.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);
  display.clearDisplay();

  pinMode(STM_BTN1, INPUT_PULLUP); // инициализируем кнопочки
  pinMode(LORA_PA0, INPUT);
  pinMode(LORA_RST, INPUT);

  // инициализируем софтовые/хардовые serial-ы
  MySerial1.begin(115200); // обычный serial
  S_Serial.begin(115200);
  MySerial3.begin(115200); // serial SIM868

  // инициализируем  пины SIM868
  pinMode(SIM_SLEEP, INPUT);
  pinMode(SIM_PWRK, OUTPUT);

  int NUM_KEYS = 2;
  char buff;
  char buff2[NUM_KEYS];

  delay(5000); ///////// нужен чтобы успеть открыть монитор порта потом удалить!!!!!!

  E52_default_init(); // инициализируем Е52 по дефолту

  SIM868_Power_SW(SIM_PWRK); // включаем SIM868

  SIM868_GPS_Power_Up(); // включаем GPS
}

void loop()
{ // ======================== LOOP ===============================

  unsigned long start_time = millis(); // таймер
  int butt_count = 1;
  int status_count = 1;
  int SRC_ADDR = 1;
  int power_counter = 22;

  bool connect_flag = 0;
  
  String Module_ADDR = Set_E52_ADDR();  // устанавливаем и запоминаем адрес Е52

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("    === " + Module_ADDR + " ===");

  display.print("Power (3), dBm: ");
  int Power_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
  int Power_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
  display.println("22");

  display.print("!!Pause (2), ms: ");
  int Pause_Xpos = display.getCursorX(); // позиция Х курсора при написании паузы
  int Pause_Ypos = display.getCursorY(); // позиция Y курсора при написании паузы
  display.println("600");

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

  display.display();

  MySerial3.println("AT+CIPCLOSE");
  delay(500);
  read_SIM868();

  while (true)
  {
    //  ========================= MODE AND SENDING ================================
    if ((millis() - start_time) >= 5000)
    {
      start_time = millis();
      MySerial1.println("==================================================================================");

      String data_transmitt = get_telemetry(Module_ADDR, status_count);

      if (digitalRead(STM_SW6) == false)
      { // ======================== MESH ============================
        draw_pos(Mode_Xpos, Mode_Ypos, "Mesh");
        send_to_mesh_E52(data_transmitt); // отправляем пакет (Если что то с модулем: data_transmitt = "GL 6666 56.452051 84.962577 174.967 1.5 190.4 1 2";)
      }

      else if (digitalRead(STM_SW6) == true)
      { // ======================== INTERNET ===========================
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

    if (digitalRead(STM_BTN1) == false)
    { // ========================== SPEED/RANGE ======================================
      butt_count = Next_SR(butt_count, SR_Xpos, SR_Ypos);
    }

    if (digitalRead(LORA_RST) == false)
    { // ========================== STATUS ========================== 
      status_count = Next_status(status_count, Stat_Xpos, Stat_Ypos);
    }

    if (digitalRead(STM_SW3) == true)
    { // ========================== POWER ========================== 
      power_counter = Next_power(power_counter, Power_Xpos, Power_Ypos);
    }

    delay(500);
  }
}