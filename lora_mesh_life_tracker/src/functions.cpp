#include "functions.h"

Adafruit_BMP280 bmp;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
serialEEPROM myEEPROM(0x50, 32768, 64);

uint8_t UART1_TX = PA9;
uint8_t UART1_RX = PA10;
HardwareSerial MySerial1(UART1_RX, UART1_TX);

// On-Board LEDs
uint8_t LED_PC13 = PC13;

// LoRa LEDs
uint8_t LED_LED1 = PB8;
uint8_t LED_LED2 = PB9;

// SIM868 UART
uint8_t UART3_TX = PB10;
uint8_t UART3_RX = PB11;
HardwareSerial MySerial3(UART3_RX, UART3_TX);

// On-Board buttons                                                               // КНОПОЧКИ
uint8_t STM_LT = PB3;
uint8_t STM_DN = PA15;
uint8_t STM_OK = PA12;
uint8_t STM_RT = PA11;
uint8_t STM_UP = PA8;

// On-Board switches
uint8_t STM_SW1 = PC14;
uint8_t STM_SW2 = PC15;
uint8_t STM_SW3 = PA0;
uint8_t STM_SW4 = PA1;
uint8_t STM_SW5 = PB0;
uint8_t STM_SW6 = PB1;

// SIM868 GPIO
uint8_t SIM_SLEEP = PA4;
uint8_t SIM_PWRK = PB15;

uint8_t switches[] = {
    STM_SW1,
    STM_SW2,
    STM_SW3,
    STM_SW4,
    STM_SW5,
    STM_SW6,
};

String APN, power, rate, dst_addr, param5;

const int switchesSize = sizeof(switches) / sizeof(uint8_t);
uint8_t switchesState[switchesSize] = {0};

// LoRa URAT
uint8_t UART2_TX = PA2;
uint8_t UART2_RX = PA3;
HardwareSerial S_Serial(UART2_RX, UART2_TX);

float Preshure[2]{101325, 101325};
float alt_rate_massiv[5]{0,0,0,0,0};
int time_massiv[2]{0, 0};
String altitude_rate = "E";
unsigned long alt_rate_time = millis();

//======================================================= ФУНКЦИИ ========================================================================
void send_command(String command)                                                    // функиця отправки AT-команды в Е52
{ 
  S_Serial.println(command);
  delay(100);
  while (S_Serial.available())
  {
    byte buff123 = S_Serial.read();
    MySerial1.write(buff123);
  }
}

void set_power(int power)                                                            // функция настройки мощности
{ 
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

void read_SSerial()                                                                  // функция чтения Soft UART с задержкой
{ 
  delay(100);
  while (S_Serial.available())
  {
    byte buff123 = S_Serial.read();
    MySerial1.write(buff123);
  }
}

int set_pause(int pause)                                                             // функция установки паузы передачи
{ 
  MySerial1.print("Пауза, мс: ");
  MySerial1.println(pause);
  int test_delay = pause - 300;
  return (test_delay);
}

void set_rs(int rs)                                                                  // функция изменения параметра скорость/дальность
{ 
  String range_speed = String(rs);
  String at = "AT+RATE=";
  at.concat(range_speed);
  S_Serial.println(at);
  MySerial1.print("Скорость/Дальность = ");
  MySerial1.println(range_speed);
  read_SSerial();
}

void set_SRC_ADDR(int SRC)                                                           // функция изменения собственного адреса
{ 
  String range_speed = String(SRC);
  String at = "AT+SRC_ADDR=";
  String save = ",1";
  at.concat(range_speed);
  at.concat(save);
  S_Serial.println(at);
  MySerial1.print("Собственный адрес = ");
  MySerial1.println(range_speed);

  while (S_Serial.available())
  {
    byte buff123 = S_Serial.read();
    MySerial1.write(buff123);
  }
}

void read_SIM868()                                                                   // функция чтения ответа от SIM868
{
  while (MySerial3.available())
  {
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
}

void send_SIM868(String command)                                                     // отправка АТ команды в sim
{
  MySerial3.println(command);
  read_SIM868();
  delay(100);
}

void draw_pos(int x_pos, int y_pos, String text)                                     // функция отрисовки по позиции, закрашивая строку
{
  display.setCursor(x_pos, y_pos);
  display.fillRect(x_pos, y_pos, 128, 8, SSD1306_BLACK);
  display.print(text);
  display.display();
}

bool send_to_server_SIM868(String dataTransmit)                                      // отправляем данные на сервер используя SIM868
{
  MySerial1.println("Sending data to server ===>");
  MySerial3.println("AT+CIPSEND=" + String(dataTransmit.length()));
  MySerial1.print("Sizeof= ");
  MySerial1.println("AT+CIPSEND=" + String(dataTransmit.length()));
  delay(100);

  String connection_status = MySerial3.readString();
  
  /*
  MySerial1.print("Connection Status =");
  MySerial1.println(connection_status);
  */

//read_SIM868();
  MySerial3.println(dataTransmit); // отправляем пакет // если нету модуля то заменить аргументы в скобках на строку: " 1111 56.45205 84.96131 450 1.5 50 2"
  MySerial1.print("pack = ");
  MySerial1.println(dataTransmit);
  
  if(connection_status.indexOf("ERROR")!=-1)
  {
    return(0);
  }
  else
  {
    return(1);
  }

 //read_SIM868();
  
}

bool check_connect_to_server()                                                       // функция проверки соединения с сервером

{
  bool connect_flag = 0;
  String connect ="connect";
  char CONNECT_buf[150] = "Nothing";
  int CONNECT_buf_index = 0;

  while (MySerial3.available())
          {
            byte buff123 = MySerial3.read();
            MySerial1.write(buff123);
            CONNECT_buf[CONNECT_buf_index] = buff123;
            CONNECT_buf_index++;
          }
          connect = String(CONNECT_buf);
          MySerial1.print("connect = ");
          MySerial1.println(connect);

          if (connect.lastIndexOf("FAIL") != -1)
          {
            MySerial1.println("CONNECT TO SERVER FAIL");
            connect_flag = 0;
          }
          else if (connect.lastIndexOf("CONNECT OK") != -1)
          {
            MySerial1.println("CONNECT TO SERVER OK");
            connect_flag = 1;
          }

          return(connect_flag);
}

void try_connect_to_server()                                                         // выполняем попытку подключиться к серверу
{
  MySerial3.println("ATE0");
          while (MySerial3.available())
          read_SIM868();
          delay(100);

          MySerial3.println("AT+CIPCLOSE");                          // закрываем старые TCP соединения 
          delay(500);
          read_SIM868();

          MySerial3.println("AT+CSQ");
          read_SIM868();
          delay(100);

          MySerial3.println("AT+CREG?");
          read_SIM868();
          delay(100);

          MySerial3.println("AT+CGATT?");
          read_SIM868();
          delay(100);

          MySerial3.println("AT+CSTT=\"CMNET\"");
          read_SIM868();
          delay(100);

          MySerial3.println("AT+CIICR");
          delay(100);

          MySerial3.println("AT+CIFSR");
          read_SIM868();

          MySerial3.println("AT+CIPSTART=\"TCP\",\"80.74.24.27\",5000");
          delay(3000);
}

void setup_gprs_parameter()                                                          // настраиваем ппараметры GPRS (APN)
{  
  delay(15000);
  read_SIM868();
  MySerial3.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  read_SIM868();
  delay(100);
  //MySerial3.println("AT+SAPBR=3,1,\"APN\",\"internet.tele2.ru\"");      !!!!!!!!!!!!!
  read_SIM868();
  delay(100);
  MySerial3.println("AT+SAPBR=1,1");
  read_SIM868();
  delay(100);
}

int Next_status(int status_count, int Stat_Xpos, int Stat_Ypos)                      // выполняем смену статуса
{
  status_count++;
      if (status_count == 1)
      {
        draw_pos(Stat_Xpos, Stat_Ypos, "Ground");
      }
      if (status_count == 2)
      {
        draw_pos(Stat_Xpos, Stat_Ypos, "Sky");
      }
      if (status_count == 3)
      {
        draw_pos(Stat_Xpos, Stat_Ypos, "Picked up");
      }
      if (status_count == 4)
      {
        draw_pos(Stat_Xpos, Stat_Ypos, "SOS");
        status_count = 0;
      }
      delay(200);
      return(status_count);

}

int Next_SR(int butt_count, int SR_Xpos, int SR_Ypos)                                // меняем параметр скорость/дальность
{
  butt_count++;
      MySerial1.print(butt_count);
      if (butt_count == 1)
      {
        set_rs(0);
        MySerial1.println("S/R=0");
        draw_pos(SR_Xpos, SR_Ypos, "0");
      }
      if (butt_count == 2)
      {
        set_rs(1);
        MySerial1.println("S/R=1");
        draw_pos(SR_Xpos, SR_Ypos, "1");
      }
      if (butt_count == 3)
      {
        set_rs(2);
        butt_count = 0;
        MySerial1.println("S/R=2");
        draw_pos(SR_Xpos, SR_Ypos, "2");
      }
      delay(200);
      return(butt_count);
}

String Set_E52_ADDR()                                                                // устанавливаем адрес Е52 по последним 4-м ицфрам МАС адреса
{
  //read_SSerial();
  send_command("AT+SRC_ADDR=1234,1");
  char MAC_buff[50] = "1010";
  int MAC_buff_index = 0;

  S_Serial.print("AT+MAC=?");
  MySerial1.print("AT+MAC=?");
  delay(100);

  while (S_Serial.available())
  {
    byte buff123 = S_Serial.read();
    MySerial1.write(buff123);
    MAC_buff[MAC_buff_index] = buff123;
    MAC_buff_index++;
  }
  String MAC_addr = String(MAC_buff);
  String Module_ADDR = MAC_addr.substring(MAC_addr.indexOf(",")+1);
  Module_ADDR = Module_ADDR.substring(Module_ADDR.length()-6,Module_ADDR.length()-2);

  MySerial1.print("Module_addr=");
  MySerial1.print(Module_ADDR);
  send_command("AT+SRC_ADDR=" + Module_ADDR + ",1");
  

  return(Module_ADDR);
}

void send_to_mesh_E52(String data_transmitt)                                         // отправляем данные в меш при помщи Е52
{
  S_Serial.println(data_transmitt); 
  MySerial1.print("pack = ");
  MySerial1.println(data_transmitt);
}

void  E52_default_init()                                                              // инициализируемся по дефолту
{                       
  send_command("AT+POWER=22,0");     // устанавливаем базовую мощность
  send_command("AT+DST_ADDR=404,0"); // задаем целевой адрес
  send_command("AT+OPTION=1,0");     // задаем режим передачи (1 - unicast (одноадресная))
  send_command("AT+RATE=0");         // устанавливаем параметр скорость/дальность
}

void SIM868_GPS_Power_Up()                                                           // включаем GPS
{
  MySerial3.write("AT+CGNSPWR=1\n"); // подаем питание на GPS
  delay(100);
  read_SIM868();
}

void SIM868_Power_SW(int SIM868_PWR_Pin)                                             // включаем/выключаем Е52
{
  pinMode(SIM868_PWR_Pin, OUTPUT);
  digitalWrite(SIM868_PWR_Pin, HIGH);
  delay(100);
  digitalWrite(SIM868_PWR_Pin, LOW);
  delay(1000);
  digitalWrite(SIM868_PWR_Pin, HIGH);
  delay(3000);
  pinMode(SIM868_PWR_Pin, INPUT);
}

int Next_power(int power_counter, int Power_Xpos, int Power_Ypos)                    // переключаем мощность Е52
{
  power_counter--;
  set_power(power_counter); // устанавливаем мощность
  draw_pos(Power_Xpos, Power_Ypos, String(power_counter));

  if (power_counter == -9)
  {
    power_counter = 23;
  }
  read_SSerial();
  delay(200);
  return(power_counter);
}

float get_altitude_rate(float P, float P_pred, int t, int t_pred)                    // получаем скороподъемность
{
  float R = 8.134;                              // газовая постоянная 
  float T = bmp.readTemperature() + 273.15;     // температура в кельвинах
  float g = 9.81;                               // ускорение свободного падения 
  float M = 0.029;                              // молярная масса воздуха

  return((((R*T)/(g*M))*((P_pred-P)/(P*((t/1000)-(t_pred/1000))))));  
}

String get_telemetry(String Module_ADDR, int status_count, String altitude_rate )    // получаем телеметрию
{
  String lattitude = "lattitude";
  String lontitude = "lontitude";
  String altitude = "altitude";
  String speed = "speed";
  String course = "course";
  char GPS_buff[150] = "Nothing";
  int GPS_buff_index = 0;
  int index1 = 0;
  String GPS_str = "GPS";

      // =============================== ПОЛУЧЕНИЕ ТЕЛЕМЕТРИИ ==============================
      MySerial1.print("Get GPS:");
      read_SIM868();                              // на всякиий случай, перед получением корд читаем юарт, чтобы буфер был гарантированно пуст
      MySerial3.write("AT+CGNSINF\n");
      delay(5);
      while (MySerial3.available())
      {
        GPS_str = MySerial3.readString();
        MySerial1.println(GPS_str);
      }
      // GPS_str = "1,1,20240208183233.000,55.643222,37.336658,336.55,0.00,323.0,1,,0.9,1.2,0.8,,12,10,9,,33,,";//подмена для отладки

      GPS_buff_index = 0;

      index1 = (GPS_str.indexOf(".") + 5);
      GPS_str = GPS_str.substring(index1);

      lattitude = GPS_str.substring(0, GPS_str.indexOf(","));
      lontitude = GPS_str.substring(GPS_str.indexOf(",") + 1);
      altitude = lontitude.substring(lontitude.indexOf(",") + 1);
      speed = altitude.substring(altitude.indexOf(",") + 1);
      course = speed.substring(speed.indexOf(",") + 1);

      lontitude = lontitude.substring(0, lontitude.indexOf(","));
      altitude = altitude.substring(0, altitude.indexOf(","));
      speed = speed.substring(0, speed.indexOf(","));
      course = course.substring(0, course.indexOf(","));

      MySerial1.println(lattitude);
      MySerial1.println(lattitude.length());
      MySerial1.println(lontitude);
      MySerial1.println(lontitude.length());

      if (lattitude.length() < 7)
      {
        lattitude = "E";
      }
      if (lontitude.length() < 7)
      {
        lontitude = "E";
      }
      else if (lontitude.length() > 9){
        lontitude = "E";
      }
      if (altitude.length() < 3)
      {
        altitude = "E";
      }
      if ((speed.length() < 3) or (speed.startsWith(".") == true))
      {
        speed = "E";
      }
      if (course.length() < 1)
      {
        course = "E";
      }
      String data_transmitt = " "+ Module_ADDR + " " + lattitude + " " 
      + lontitude + " " + altitude + " " + altitude_rate + " " + speed + " " 
      + status_count + " " + course;
       
      return(data_transmitt);
}

void setup_bmp()                                                                     // установка параметров BMP
{
  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void get_setup_from_ESP()                                                            // получение настроек по меш от ESP
{
 // MySerial1.println("MODE SETTINGS");
  if(S_Serial.available()){
    String settings_message = S_Serial.readString();
    MySerial1.println("======= SETTINGS FROM ESP =======");
    if(settings_message.indexOf("ST") != -1){
      MySerial1.println(settings_message);
      settings_message = settings_message.substring(settings_message.indexOf(" ")+1);
      APN = "\"" + settings_message.substring(0, settings_message.indexOf(" "))+"\"";
      MySerial1.println(APN);
      settings_message = settings_message.substring(settings_message.indexOf(" ")+1);
      power = settings_message.substring(0, settings_message.indexOf(" "));
      MySerial1.println(power);
      settings_message = settings_message.substring(settings_message.indexOf(" ")+1);
      rate = settings_message.substring(0, settings_message.indexOf(" "));
      MySerial1.println(rate);
      settings_message = settings_message.substring(settings_message.indexOf(" ")+1);
      dst_addr = settings_message.substring(0, settings_message.indexOf(" "));
      MySerial1.println(dst_addr);
      settings_message = settings_message.substring(settings_message.indexOf(" ")+1);
      param5 = settings_message.substring(0, settings_message.indexOf(";"));
      MySerial1.println(param5);
      
      send_SIM868("AT+SAPBR=3,1,\"APN\"," + APN);
      send_command("AT+POWER="+ power +",1");
      send_command("AT+RATE="+rate); 
      send_command("AT+DST_ADDR="+dst_addr+",1");
      read_SIM868();
    }        
  }
  //delay(1000);
}

String get_ar_with_filter(int ALTR_Xpos, int ALTR_Ypos)                              // получаем и фильтруем скороподъемность
{
  alt_rate_time = millis();

      time_massiv[0] = time_massiv[1];
      time_massiv[1] = millis();
      Preshure[0] = Preshure[1];
      Preshure[1] = bmp.readPressure();

      alt_rate_massiv[4]=alt_rate_massiv[3]; 
      alt_rate_massiv[3]=alt_rate_massiv[2]; 
      alt_rate_massiv[2]=alt_rate_massiv[1]; 
      alt_rate_massiv[1]=alt_rate_massiv[0];        // криворукий циклический буффер !!! исправить
      alt_rate_massiv[0] = get_altitude_rate(Preshure[1], Preshure[0], time_massiv[1], time_massiv[0]);

      float alt_rate = ((alt_rate_massiv[0]+alt_rate_massiv[1]+alt_rate_massiv[2]+alt_rate_massiv[3]+alt_rate_massiv[4])/5);
     
      draw_pos(ALTR_Xpos, ALTR_Ypos, String(alt_rate));
      altitude_rate = String(alt_rate);

      return(altitude_rate);
}

void init_pinout_and_display()                                                       // инициализируем пины и настройки экранчика
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) // инициализация дисплея    !!!!! спросить у Александра почему так !!!!!
  {
    MySerial1.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);
  display.clearDisplay();
  

  pinMode(STM_LT, INPUT); // инициализируем кнопочки
  pinMode(STM_DN, INPUT);
  pinMode(STM_OK, INPUT);
  pinMode(STM_RT, INPUT);
  pinMode(STM_UP, INPUT);
  
  pinMode(STM_SW1, INPUT_PULLUP);
  pinMode(STM_SW2, INPUT_PULLUP);
  pinMode(STM_SW3, INPUT_PULLUP);
  pinMode(STM_SW4, INPUT_PULLUP);
  pinMode(STM_SW5, INPUT_PULLUP);
  pinMode(STM_SW6, INPUT_PULLUP);

  // инициализируем софтовые/хардовые serial-ы
  MySerial1.begin(115200); // обычный serial
  S_Serial.begin(115200);  //
  MySerial3.begin(115200); // serial SIM868

  // инициализируем  пины SIM868
  pinMode(SIM_SLEEP, INPUT);
  pinMode(SIM_PWRK, OUTPUT);
}

void init_board()                                                                    // инициализируем плату
{                                                                  
  delay(1000);                                                // нужно для включения экрана, без этого почему то не успевает
  init_pinout_and_display();
  E52_default_init(); // инициализируем Е52 по дефолту
  setup_bmp();
  SIM868_Power_SW(SIM_PWRK); // включаем SIM868      несколько перезагрузок нужно для симки (без этого не устанавливается APN)
  //SIM868_Power_SW(SIM_PWRK); // вылючаем SIM868
  //SIM868_Power_SW(SIM_PWRK); // включаем SIM868
  SIM868_GPS_Power_Up(); // включаем GPS
  setup_gprs_parameter();  // настраиваем APN пока что здесь, потом надо чтобы менялся с базы
}

Display_coordinates init_menu(String Module_ADDR)                                    // отрисовываем меню
{
  Display_coordinates coordinates = {};
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("    === " + Module_ADDR + " ===");  // !!!!!!!!!!!!! можно попробовать заменить все инты на структуру и потом её return();

  display.print("Power, dBm: ");
  coordinates.Power_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
  coordinates.Power_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
  display.println("22");

  display.print("S/R (1-2): ");
  coordinates.SR_Xpos = display.getCursorX(); // позиция Х курсора при написании Speed/Rate
  coordinates.SR_Ypos = display.getCursorY(); // позиция Y курсора при написании Speed/Rate
  display.println("0");

  display.print("Mode (6): ");
  coordinates.Mode_Xpos = display.getCursorX(); // позиция Х курсора при написании режима
  coordinates.Mode_Ypos = display.getCursorY(); // позиция Y курсора при написании режима
  display.println("Not");

  display.print("Status: ");
  coordinates.Stat_Xpos = display.getCursorX(); // позиция Х курсора при написании статуса
  coordinates.Stat_Ypos = display.getCursorY(); // позиция Y курсора при написании статуса
  display.println("Ground");

  display.print("ALT_R: ");
  coordinates.ALTR_Xpos = display.getCursorX(); // позиция Х курсора при написании статуса
  coordinates.ALTR_Ypos = display.getCursorY(); // позиция Y курсора при написании статуса
  display.println("N/A");

  display.print("Pause(3-4): ");
  coordinates.pause_Xpos = display.getCursorX(); // позиция Х курсора при написании статуса
  coordinates.pause_Ypos = display.getCursorY(); // позиция Y курсора при написании статуса
  display.println("3-5");

  display.display();

  return coordinates;
}

