#include "functions.h"
#include "configuration.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
serialEEPROM myEEPROM(0x50, 32768, 64);

uint8_t UART1_TX = PA9;
uint8_t UART1_RX = PA10;
// HardwareSerial MySerial1(UART1_RX, UART1_TX);

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

uint8_t UART2_TX = PA2;
uint8_t UART2_RX = PA3;
HardwareSerial S_Serial(UART2_RX, UART2_TX);

//======================================================= ФУНКЦИИ ========================================================================
void read_SIM868() // функция чтения ответа от SIM868
{
  while (MySerial3.available())
  {
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
  }
}

void draw_pos(int x_pos, int y_pos, String text) // функция отрисовки по позиции, закрашивая строку
{
  display.setCursor(x_pos, y_pos);
  display.fillRect(x_pos, y_pos, 128, 8, SSD1306_BLACK);
  display.print(text);
  display.display();
}

void send_to_server_SIM868(String dataTransmit) // отправляем данные на сервер используя SIM868
{
  MySerial1.println("Sending data to server ===>");
  MySerial3.println("AT+CIPSEND=" + String(dataTransmit.length()));
  MySerial1.print("Sizeof= ");
  MySerial1.println("AT+CIPSEND=" + String(dataTransmit.length()));
  delay(100);
  read_SIM868();
  MySerial3.println(dataTransmit); // отправляем пакет // если нету модуля то заменить аргументы в скобках на строку: "56.45205 84.96131 450 1.5 50 2"
  MySerial1.print("pack = ");
  MySerial1.println(dataTransmit);
  read_SIM868();
}

bool check_connect_to_server() // функция проверки соединения с сервером
{
  bool connect_flag = 0;
  String connect = "connect";
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
    MySerial1.println("Not connect(((((((");
    connect_flag = 0;
  }
  else if (connect.lastIndexOf("CONNECT OK") != -1)
  {
    MySerial1.println("CONNECT)))))))))");
    connect_flag = 1;
  }

  return (connect_flag);
}

void try_connect_to_server() // выполняем попытку подключиться к серверу
{
  MySerial3.println("ATE0");
  while (MySerial3.available())
    read_SIM868();
  delay(100);

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

  MySerial3.println("AT+CIPSTART=\"TCP\",\"103.90.75.178\",5000");
  delay(3000);
}

int Next_status(int status_count, int Stat_Xpos, int Stat_Ypos) // выполняем смену статуса
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
  delay(100);
  return (status_count);
}

void SIM868_GPS_Power_Up() // включаем GPS
{
  MySerial3.write("AT+CGNSPWR=1\n"); // подаем питание на GPS
  delay(100);
  read_SIM868();
}

void SIM868_Power_SW(int SIM868_PWR_Pin) // включаем/выключаем Е52
{
  digitalWrite(SIM868_PWR_Pin, HIGH);
  // digitalWrite(LED_PC13, LOW);
  delay(100);
  digitalWrite(SIM868_PWR_Pin, LOW);
  // digitalWrite(LED_PC13, HIGH);
  delay(1000);
  digitalWrite(SIM868_PWR_Pin, HIGH);
  // digitalWrite(LED_PC13, LOW);
  delay(3000);
}

String get_telemetry(String Module_ADDR, int status_count) // получаем телеметрию
{
  String lattitude = "lattitude";
  String lontitude = "lontitude";
  String altitude = "altitude";
  String speed = "speed";
  String course = "course";
  String wrong_data = "1.5";
  char GPS_buff[150] = "Nothing";
  int GPS_buff_index = 0;
  int index1 = 0;
  String GPS_str = "GPS";

  // =============================== ПОЛУЧЕНИЕ ТЕЛЕМЕТРИИ ==============================
  MySerial1.println("Get GPS");
  read_SIM868(); // на всякиий случай, перед получением корд читаем юарт, чтобы буфер был гарантированно пуст
  MySerial3.write("AT+CGNSINF\n");
  delay(10);
  while (MySerial3.available())
  {
    byte buff123 = MySerial3.read();
    MySerial1.write(buff123);
    GPS_buff[GPS_buff_index] = buff123;
    GPS_buff_index++;
  }
  GPS_str = String(GPS_buff);
  // GPS_str = "1,1,20240208183233.000,55.643222,37.336658,336.55,0.00,323.0,1,,0.9,1.2,0.8,,12,10,9,,33,,";//подмена для отладки

  GPS_buff_index = 0;

  index1 = (GPS_str.indexOf(".") + 5);
  GPS_str = GPS_str.substring(index1);
  /*MySerial1.print("GPS =");
  MySerial1.println(GPS_str);*/

  lattitude = GPS_str.substring(0, GPS_str.indexOf(","));
  lontitude = GPS_str.substring(GPS_str.indexOf(",") + 1);
  altitude = lontitude.substring(lontitude.indexOf(",") + 1);
  speed = altitude.substring(altitude.indexOf(",") + 1);
  course = speed.substring(speed.indexOf(",") + 1);

  lontitude = lontitude.substring(0, lontitude.indexOf(","));
  altitude = altitude.substring(0, altitude.indexOf(","));
  speed = speed.substring(0, speed.indexOf(","));
  course = course.substring(0, course.indexOf(","));

  MySerial1.print("\n");
  MySerial1.print("Lat= ");
  MySerial1.println(lattitude);
  MySerial1.print("Lon= ");
  MySerial1.println(lontitude);
  MySerial1.print("Alt= ");
  MySerial1.println(altitude);
  MySerial1.print("Spd= ");
  MySerial1.println(speed);
  MySerial1.print("Crs= ");
  MySerial1.println(course);

  MySerial1.print("len_lat= ");
  MySerial1.println(lattitude.length());
  MySerial1.print("len_lon= ");
  MySerial1.println(lontitude.length());
  MySerial1.print("len_alt= ");
  MySerial1.println(altitude.length());
  MySerial1.print("len_spd= ");
  MySerial1.println(speed.length());
  MySerial1.print("len_crs= ");
  MySerial1.println(course.length());

  if (lattitude.length() < 7)
  {
    lattitude = "-1";
  }
  if (lontitude.length() < 7)
  {
    lontitude = "-1";
  }
  else if (lontitude.length() > 9)
  {
    lontitude = "-1";
  }
  if (altitude.length() < 3)
  {
    altitude = "-1";
  }
  if (speed.length() < 3)
  {
    speed = "-1";
  }
  if (course.length() < 1)
  {
    course = "-1";
  }
  String data_transmitt = "GL " + Module_ADDR + " " + lattitude + " " + lontitude + " " + altitude + " " + wrong_data + " " + speed + " " + status_count + " " + course;

  return (data_transmitt);
}
