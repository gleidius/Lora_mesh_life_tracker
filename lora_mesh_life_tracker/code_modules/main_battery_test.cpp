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

void setup(){                            //========================== SETUP ===========================

  for (int i = 0; i < switchesSize; i++)      // инициализируем свичи
  {
    pinMode(switches[i], INPUT_PULLUP);
  }

    pinMode(STM_BTN1, INPUT);                 // инициализируем кнопочки
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
  send_command("AT+POWER=22,0");                           // устанавливаем базовую мощность
  send_command("AT+SRC_ADDR=11,0");                        // задаем собственный адрес
  send_command("AT+DST_ADDR=25,0");                        // задаем целевой адрес
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
  /*
  MySerial3.write("AT+CSCLK=1\n");
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
  digitalWrite(SIM_SLEEP, HIGH);
  delay(3000);
  digitalWrite(SIM_SLEEP, LOW);
*/
  MySerial1.write("Init GPS");
  MySerial3.print("AT+CGNSPWR=1\n");
  delay(3000); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }

  display.setCursor(0, 0);
  display.fillRect(0, 0, 128, 64, SSD1306_WHITE);
  display.display();
  
  // SENDING TO SERVER SETTINGS
  MySerial3.print("AT+CSQ\n");
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
  MySerial3.print("AT+CREG?\n");
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
  MySerial3.print("AT+CGATT?\n");
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
  MySerial3.print("AT+CSTT=\"CMNET\"\n");
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
  MySerial3.print("AT+CIICR\n");
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
  MySerial3.print("AT+CIFSR\n");
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
  MySerial3.print("AT+CIPSTART=\"TCP\",\"103.90.75.178\",8000\n");
  delay(3000); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
    
}


void loop(){                            // ======================== LOOP ===============================

  //                         ======================== SIM868 ============================
  
  
  while(true){

// GPS
  MySerial1.println("=========================================");
  
    MySerial1.println("GET GPS:");
    MySerial3.print("AT+CGNSINF\n");
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }

  
  // SENDING TO SERVER
  MySerial1.print("\n");
  MySerial1.println("SEND TO SERVER:");
  MySerial3.write("AT+CIPSEND=30\n");
  delay(100); 
  while(MySerial3.available()){
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
  MySerial3.write("56.45205 84.96131 450 1.5 50 2");
  delay(900); 





  //                     ============================= E52 =============================
  MySerial1.println("\n");
  MySerial1.println("SEND TO E52:");
  S_Serial.println("56.45205 84.96131 450 1.5 50 2");               // отправляем пакет в режиме теста
  delay(300); 
  while(S_Serial.available()){
    byte buff123 = S_Serial.read();
    MySerial1.write(buff123);
  }
  
  delay(800);
  //delay(5000);
  }
}