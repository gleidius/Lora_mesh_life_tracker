#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <serialEEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

static uint8_t UART1_TX = PA9;
static uint8_t UART1_RX = PA10;
HardwareSerial MySerial1(UART1_RX, UART1_TX);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
serialEEPROM myEEPROM(0x50, 32768, 64);

// On-Board LEDs
uint8_t LED_PC13 = PC13;

// LoRa LEDs
uint8_t LED_LED1 = PB8;
uint8_t LED_LED2 = PB9;

// I2C
// uint8_t SDA = PB7;
// uint8_t SCL = PB6;

// UART1
//uint8_t UART1_TX = PA9;
//uint8_t UART1_RX = PA10;

// LoRa UART
//uint8_t UART2_TX = PA2;
//uint8_t UART2_RX = PA3;

// SIM868 UART
uint8_t UART3_TX = PB10;
uint8_t UART3_RX = PB11;

//HardwareSerial MySerial1(UART1_RX, UART1_TX);
//HardwareSerial MySerial2(UART2_RX, UART2_TX);
HardwareSerial MySerial3(UART3_RX, UART3_TX);
// On-Board buttons                                                               // КНОПОЧКИ
uint8_t STM_BTN1 = PB0;
uint8_t LORA_PA0 = PB1;
uint8_t LORA_RST = PA6;

// uint8_t STM_SW1 = PC13;                                                        // ПЕРЕКЛЮЧАТЕЛИ
uint8_t STM_SW2 = PC14;
uint8_t STM_SW3 = PC15;
uint8_t STM_SW4 = PA0;
uint8_t STM_SW5 = PA1;
uint8_t STM_SW6 = PA7;

// SIM868 GPIO
uint8_t SIM_SLEEP = PA4;
uint8_t SIM_PWRK = PA5;

uint8_t switches[] = {
  // STM_SW1,
  STM_SW2,
  STM_SW3,
  STM_SW4,
  STM_SW5,
  STM_SW6,
};

const int switchesSize = sizeof(switches) / sizeof(uint8_t);
uint8_t switchesState[switchesSize] = {0};

static uint8_t UART2_TX = PA2;
static uint8_t UART2_RX = PA3;
static HardwareSerial S_Serial(UART2_RX, UART2_TX);

                              // ================================= FUNCTION ====================================
  void send_command(String command){                // функиця отправки AT-команды в Е52
    S_Serial.println(command);
    delay(100); 
    while(S_Serial.available()){
      byte buff123 = S_Serial.read();
      MySerial1.write(buff123);
    }}

  void set_power(int power){                        // функция настройки мощности
      String pw = String(power);
      String at = "AT+POWER=";
      String zero = ",0";
      at.concat(pw);
      at.concat(zero);
      S_Serial.println(at);
      MySerial1.print("Мощность: ");
      String dbm = " дБм";
      pw.concat(dbm);
      MySerial1.println(pw);
  }
  
  void read_SSerial(){                              // функция чтения Soft UART с задержкой
    delay(100);
    while(S_Serial.available()){
      byte buff123 = S_Serial.read();
      MySerial1.write(buff123);  
  }}
  
  int set_pause(int pause){                         // функция установки паузы передачи
    MySerial1.print("Пауза, мс: ");
    MySerial1.println(pause);
    int test_delay=pause-300;
    return(test_delay);
    }
  
  void set_rs(int rs){                              // функция изменения параметра скорость/дальность
      String range_speed = String(rs);
      String at = "AT+RATE=";
      at.concat(range_speed);
      S_Serial.println(at);
      MySerial1.print("Скорость/Дальность = ");
      MySerial1.println(range_speed);
    }

    void set_SRC_ADDR(int SRC){                     // функция изменения собственного адреса 
      String range_speed = String(SRC);
      String at = "AT+SRC_ADDR=";
      String save = ",1";
      at.concat(range_speed);
      at.concat(save);
      S_Serial.println(at);
      MySerial1.print("Собственный адрес = ");
      MySerial1.println(range_speed);

      while(S_Serial.available()){
        byte buff123 = S_Serial.read();
        MySerial1.write(buff123);
      }
    }

void setup(){                            //========================== SETUP ===========================

  for (int i = 0; i < switchesSize; i++)      // инициализируем свичи
  {
    pinMode(switches[i], INPUT_PULLUP);
  }

    pinMode(STM_BTN1, INPUT_PULLUP);                 // инициализируем кнопочки
    pinMode(LORA_PA0, INPUT);
    pinMode(LORA_RST, INPUT);


 // инициализируем софтовые/хардовые serial-ы
  MySerial1.begin(115200);                        // обычный serial 
  S_Serial.begin(115200);
  MySerial3.begin(115200);                        // serial SIM868

                                              // инициализируем  пины SIM868
    pinMode(SIM_SLEEP, INPUT);
    pinMode(SIM_PWRK, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))         // инициализация дисплея    !!!!! спросить у Александра почему так !!!!!
  {
    MySerial1.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);
  display.clearDisplay();

  int NUM_KEYS = 2;
  char buff;
  char buff2[NUM_KEYS];

    // базовые настроечки
  send_command("AT+POWER=14,0");                           // устанавливаем базовую мощность
  send_command("AT+SRC_ADDR=1,0");                        // задаем собственный адрес
  send_command("AT+DST_ADDR=404,0");                        // задаем целевой адрес
  send_command("AT+OPTION=1,0");                          // задаем режим передачи (1 - unicast (одноадресная))
  send_command("AT+RATE=0");                              // устанавливаем параметр скорость/дальность

  // инициализируем SIM868
  // pressing SIM868 PWRK pin to boot it
  digitalWrite(SIM_PWRK, HIGH);
  digitalWrite(LED_PC13, LOW);
  delay(100);
  digitalWrite(SIM_PWRK, LOW);
  digitalWrite(LED_PC13, HIGH);
  delay(1000);
  digitalWrite(SIM_PWRK, HIGH);
  digitalWrite(LED_PC13, LOW);
  delay(3000);
 
  MySerial1.println("===============================================================");
  MySerial3.write("AT+CGNSPWR=1\n");        // подаем питание на GPS
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
}

void loop(){                            // ======================== LOOP ===============================
  int butt_count = 1;
  int status_count = 1;
  int SRC_ADDR = 1;
  int power_counter = 22;
  char GPS_buff[150] = "Nothing";
  int GPS_buff_index =0;
  String GPS_str="Nothing";
  String lattitude="Error_lattitude";
  String lontitude="Error_lontitude";

  int index1=0;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(" === LIFE TRACKER ===");

  display.print("Power (3), dBm: ");
  int Power_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
  int Power_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
  display.println("22");

  display.print("!!Pause (2), ms: ");
  int Pause_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
  int Pause_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
  display.println("600");

  display.print("SRC_ADDR: ");
  int SADDR_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
  int SADDR_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
  display.println("1");

  display.print("S/R (4): ");
  int SR_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
  int SR_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
  display.println("0");

  display.print("Mode (6): ");
  int Mode_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
  int Mode_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
  display.println("Not");

  display.print("Status: ");
  int Stat_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
  int Stat_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
  display.println("Ground");

  display.display();

  while (true)
  {
                           // =============================== ПОЛУЧЕНИЕ КООРДИНАТ ==============================
      MySerial3.write("AT+CGNSINF\n");
      
      while(MySerial3.available()){
        
        byte buff123 = MySerial3.read();
        //MySerial1.write(buff123);
        GPS_buff[GPS_buff_index] = buff123;
        GPS_buff_index++;
      }
      GPS_str=String(GPS_buff);
      
      GPS_buff_index=0;

      index1 = (GPS_str.indexOf(".")+5);
      GPS_str = GPS_str.substring(index1);
      MySerial1.print("GPS =");
      MySerial1.println(GPS_str);
      
      lattitude = GPS_str.substring(0,GPS_str.indexOf(","));
      lontitude = GPS_str.substring(GPS_str.indexOf(",")+1);
      MySerial1.print("Lat= ");
      MySerial1.println(lattitude);
      MySerial1.print("Lon= ");
      MySerial1.println(lontitude);
      MySerial1.println("\n");
      String wrong_data = " 450 1.5 50";
      String space = " ";
      String GPS_TO_SEND = "";

      


    if (digitalRead(STM_SW6)== false){          // ========================= MODE AND SENDING ================================
      display.setCursor(Mode_Xpos, Mode_Ypos);
      display.fillRect(Mode_Xpos, Mode_Ypos, 128, 8, SSD1306_BLACK);
      display.print("Mesh");
      display.display();

      S_Serial.println(lattitude + space + lontitude + wrong_data + space + status_count); // отправляем пакет
      MySerial1.print("pack = ");
      MySerial1.println(lattitude + space + lontitude + wrong_data + space + status_count);
      delay(2000);
      
    }
    else if (digitalRead(STM_SW6)== true){
      display.setCursor(Mode_Xpos, Mode_Ypos);
      display.fillRect(Mode_Xpos, Mode_Ypos, 128, 8, SSD1306_BLACK);
      display.print("Internet");
      display.display();

    }

    if (digitalRead(STM_BTN1)== false){  // ========================== SPEED/RANGE ======================================
      //LORA_RST, LORA_PA0, STM_BTN1
      butt_count++;
      MySerial1.print(butt_count);
      if (butt_count == 1)
      {
        //setup_delay = 1000;
        set_rs(0);
        MySerial1.println("S/R=0");

        display.setCursor(SR_Xpos, SR_Ypos);
        display.fillRect(SR_Xpos, SR_Ypos, 128, 8, SSD1306_BLACK);
        display.print("0");
        display.display();
      }
      if (butt_count == 2)
      {
        //setup_delay = 1000;
        set_rs(1);
        MySerial1.println("S/R=1");

        display.setCursor(SR_Xpos, SR_Ypos);
        display.fillRect(SR_Xpos, SR_Ypos, 128, 8, SSD1306_BLACK);
        display.print("1");
        display.display();
      }
      if (butt_count == 3)
      {
        //setup_delay = 3000;
        set_rs(2);
        butt_count = 0;
        MySerial1.println("S/R=2");

        display.setCursor(SR_Xpos, SR_Ypos);
        display.fillRect(SR_Xpos, SR_Ypos, 128, 8, SSD1306_BLACK);
        display.print("2");
        display.display();
      }
    }

    if(digitalRead(LORA_RST)== false){      // ========================== STATUS ========================== (не забыть добавить изменение статуса в пакет)
      status_count++;
      if (status_count == 1){
        display.setCursor(Stat_Xpos, Stat_Ypos);
        display.fillRect(Stat_Xpos, Stat_Ypos, 128, 8, SSD1306_BLACK);
        display.print("Ground");           // на земле
        display.display();
        
      } 
      if (status_count == 2){
        display.setCursor(Stat_Xpos, Stat_Ypos);
        display.fillRect(Stat_Xpos, Stat_Ypos, 128, 8, SSD1306_BLACK);
        display.print("Sky");          // в небе
        display.display();
        
      } 
      if (status_count == 3){
        display.setCursor(Stat_Xpos, Stat_Ypos);
        display.fillRect(Stat_Xpos, Stat_Ypos, 128, 8, SSD1306_BLACK);
        display.print("Picked up");          // подбор
        display.display();
        
      } 
      if (status_count == 4){
        display.setCursor(Stat_Xpos, Stat_Ypos);
        display.fillRect(Stat_Xpos, Stat_Ypos, 128, 8, SSD1306_BLACK);
        display.print("SOS");          // SOS соответствует 0 в status_counter
        display.display();
        status_count = 0;
      } 
      delay(100);
    }

    if (digitalRead(LORA_PA0)== false){   // ========================= SRC_ADDR =================================
      SRC_ADDR ++;
      set_SRC_ADDR(SRC_ADDR);

      display.setCursor(SADDR_Xpos, SADDR_Ypos);
      display.fillRect(SADDR_Xpos, SADDR_Ypos, 128, 8, SSD1306_BLACK);
      display.print(SRC_ADDR);
      display.display();

      if(SRC_ADDR==100){
        SRC_ADDR = 0;
      }
    }
/*       устанавливаем паузы между передачами
if (digitalRead(STM_SW2) == true){ // устанавливаем паузы между передачами

switch_count++;
if (switch_count == 1)
{
  test_delay = set_pause(600);
  
  display.setCursor(Pause_Xpos, Pause_Ypos);
  display.fillRect(Pause_Xpos, Pause_Ypos, 128, 8, SSD1306_BLACK);
  display.print("600");
  display.display();
}
if (switch_count == 2)
{
  test_delay = set_pause(1200);
  
  display.setCursor(Pause_Xpos, Pause_Ypos);
  display.fillRect(Pause_Xpos, Pause_Ypos, 128, 8, SSD1306_BLACK);
  display.print("1200");
  display.display();
}
if (switch_count == 3)
{
  test_delay = set_pause(2000);
  
  display.setCursor(Pause_Xpos, Pause_Ypos);
  display.fillRect(Pause_Xpos, Pause_Ypos, 128, 8, SSD1306_BLACK);
  display.print("2000");
  display.display();
}
if (switch_count == 4)
{
  test_delay = set_pause(3000);
  
  display.setCursor(Pause_Xpos, Pause_Ypos);
  display.fillRect(Pause_Xpos, Pause_Ypos, 128, 8, SSD1306_BLACK);
  display.print("3000");
  display.display();
}
if (switch_count == 5)
{
  test_delay = set_pause(5000);
  switch_count = 0;
  
  display.setCursor(Pause_Xpos, Pause_Ypos);
  display.fillRect(Pause_Xpos, Pause_Ypos, 128, 8, SSD1306_BLACK);
  display.print("5000");
  display.display();
}
}
*/

if (digitalRead(STM_SW3) == true) { // переключаем мощность
power_counter--;
set_power(power_counter); // устанавливаем мощность

display.setCursor(Power_Xpos, Power_Ypos);
display.fillRect(Power_Xpos, Power_Ypos, 128, 8, SSD1306_BLACK);
display.print(power_counter);
      display.display();

      if (power_counter == -9)
      {
        power_counter = 23;
      }
      read_SSerial();
    }

  delay(500);
  }
}