#include "Screen.h"

Adafruit_SSD1306 my_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Screen::Screen(HardwareSerial &Serial1)
    : mTerminal_UART(Serial1)
{
}

void Screen::begin()
{
  if (!my_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) // инициализация дисплея    !!!!! спросить у Александра почему так !!!!!
  {
    mTerminal_UART.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  my_display.display();
  my_display.setTextSize(1);
  my_display.setTextColor(SSD1306_WHITE);
  my_display.setCursor(0, 0);
  my_display.cp437(true);
  my_display.clearDisplay();
}

void Screen::drawBitmap(int x, int y, const uint8_t bitmap[], int w, int h, uint16_t color)
{
  my_display.drawBitmap(x, y, bitmap, w, h, color);
}

void Screen::draw_in_coordinates(int x_pos, int y_pos, String text) // функция отрисовки по позиции, закрашивая строку
{
  my_display.setCursor(x_pos, y_pos);
  my_display.fillRect(x_pos, y_pos, 128, 8, SSD1306_BLACK);
  my_display.print(text);
  my_display.display();
}
void Screen::display()
{
  my_display.display();
}

void Screen::fillRect(int x_pos, int y_pos)
{
  my_display.fillRect(x_pos, y_pos, 128, 8, SSD1306_WHITE);
}

Display_coordinates Screen::draw_menu(String module_address) // отрисовываем меню
{
  Display_coordinates coordinates = {};

  my_display.clearDisplay();
  my_display.setCursor(0, 0);
  my_display.println("    === " + module_address + " ==="); // !!!!!!!!!!!!! можно попробовать заменить все инты на структуру и потом её return();

  my_display.print("Power (5), dBm: ");
  coordinates.Power_Xpos = my_display.getCursorX(); // позиция Х курсора при написании мощности
  coordinates.Power_Ypos = my_display.getCursorY(); // позиция Y курсора при написании мощности
  my_display.println("22");

  my_display.print("S/R (1-2): ");
  coordinates.SR_Xpos = my_display.getCursorX(); // позиция Х курсора при написании Speed/Rate
  coordinates.SR_Ypos = my_display.getCursorY(); // позиция Y курсора при написании Speed/Rate
  my_display.println("0");

  my_display.print("Mode (6): ");
  coordinates.Mode_Xpos = my_display.getCursorX(); // позиция Х курсора при написании режима
  coordinates.Mode_Ypos = my_display.getCursorY(); // позиция Y курсора при написании режима
  my_display.println("Not");

  my_display.print("Status: ");
  coordinates.Stat_Xpos = my_display.getCursorX(); // позиция Х курсора при написании статуса
  coordinates.Stat_Ypos = my_display.getCursorY(); // позиция Y курсора при написании статуса
  my_display.println("Ground");

  my_display.print("ALT_R: ");
  coordinates.ALTR_Xpos = my_display.getCursorX(); // позиция Х курсора при написании статуса
  coordinates.ALTR_Ypos = my_display.getCursorY(); // позиция Y курсора при написании статуса
  my_display.println("N/A");

  my_display.print("Pause(3-4): ");
  coordinates.pause_Xpos = my_display.getCursorX(); // позиция Х курсора при написании статуса
  coordinates.pause_Ypos = my_display.getCursorY(); // позиция Y курсора при написании статуса
  my_display.println("3-5");

  my_display.display();

  return coordinates;
}