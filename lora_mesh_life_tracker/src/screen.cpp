#include "screen.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

screen::screen(HardwareSerial &Serial1)
    : MySerial1(Serial1)
{
}

void screen::begin()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) // инициализация дисплея    !!!!! спросить у Александра почему так !!!!!
  {
    MySerial1.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);
  display.clearDisplay();
}
void screen::draw_pos(int x_pos, int y_pos, String text) // функция отрисовки по позиции, закрашивая строку
{
  display.setCursor(x_pos, y_pos);
  display.fillRect(x_pos, y_pos, 128, 8, SSD1306_BLACK);
  display.print(text);
  display.display();
}

Display_coordinates screen::init_menu(String Module_ADDR) // отрисовываем меню
{
  Display_coordinates coordinates = {};

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("    === " + Module_ADDR + " ==="); // !!!!!!!!!!!!! можно попробовать заменить все инты на структуру и потом её return();

  display.print("Power (5), dBm: ");
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