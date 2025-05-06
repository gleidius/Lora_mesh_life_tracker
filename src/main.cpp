// #include "functions.h"
#include "configuration.h"

// void parser()
// {
// 	String message;
// 	if (meshApp.available())
// 		message = meshApp.recieveMessage();

// 	if (message.indexOf("mesh") == 0)
// 	{
// 		DEBUG("Message from PC: ");
// 		DEBUGLN(message);
// 		if (message.indexOf("msg") == 5)
// 		{
// 			String newMessage = message.substring(9);
// 			e52.sendMessage(newMessage);
// 		}

// 		if (message.indexOf("cmd") == 5)
// 		{
// 			String newMessage = message.substring(9);
// 			e52.sendCommand(newMessage);
// 		}
// 	}
// }

// // ========================== SETUP ===========================
// void setup()
// {

// 	// pinMode(STM_BTN1, INPUT_PULLUP); // инициализируем кнопочки
// 	// pinMode(LORA_PA0, INPUT);
// 	// pinMode(LORA_RST, INPUT);

// 	// // инициализируем софтовые/хардовые serial-ы
// 	// MySerial1.begin(115200); // обычный serial
// 	S_Serial.begin(115200);
// 	Serial.begin(115200);
// 	// MySerial3.begin(115200); // serial SIM868

// 	menu.setup();

// 	delay(5000); ///////// нужен чтобы успеть открыть монитор порта потом удалить!!!!!!

// 	meshApp.setup(&Serial);
// 	meshApp.sendMessage("Start STM32");

// 	if (e52.setup(&S_Serial))
// 		meshApp.sendMessage("True");
// 	else
// 		meshApp.sendMessage("False");

// 	// // инициализируем  пины SIM868
// 	// pinMode(SIM_SLEEP, INPUT);
// 	// pinMode(SIM_PWRK, OUTPUT);

// 	// int NUM_KEYS = 2;
// 	// char buff;
// 	// char buff2[NUM_KEYS];

// 	// SIM868_Power_SW(SIM_PWRK); // включаем SIM868

// 	// SIM868_GPS_Power_Up(); // включаем GPS
// }

// // ======================== LOOP ===============================
// void loop()
// {
// 	menu.menuHandler();
// 	// meshApp.sendMessage("Hello World!");
// 	e52.sendMessage("Hello World!");
// 	parser();
// 	delay(1000);
// 	// unsigned long start_time = millis(); // таймер
// 	// int butt_count = 1;
// 	// int status_count = 1;
// 	// int SRC_ADDR = 1;
// 	// int power_counter = 22;

// 	// bool connect_flag = 0;

// 	// // String Module_ADDR = Set_E52_ADDR(); // устанавливаем и запоминаем адрес Е52
// 	// String Module_ADDR = "1234";

// 	// display.clearDisplay();
// 	// display.setCursor(0, 0);
// 	// // display.println("    === " + Module_ADDR + " ===");

// 	// display.print("Power (3), dBm: ");
// 	// int Power_Xpos = display.getCursorX(); // позиция Х курсора при написании мощности
// 	// int Power_Ypos = display.getCursorY(); // позиция Y курсора при написании мощности
// 	// display.println("22");

// 	// display.print("!!Pause (2), ms: ");
// 	// int Pause_Xpos = display.getCursorX(); // позиция Х курсора при написании паузы
// 	// int Pause_Ypos = display.getCursorY(); // позиция Y курсора при написании паузы
// 	// display.println("600");

// 	// display.print("S/R (4): ");
// 	// int SR_Xpos = display.getCursorX(); // позиция Х курсора при написании Speed/Rate
// 	// int SR_Ypos = display.getCursorY(); // позиция Y курсора при написании Speed/Rate
// 	// display.println("0");

// 	// display.print("Mode (6): ");
// 	// int Mode_Xpos = display.getCursorX(); // позиция Х курсора при написании режима
// 	// int Mode_Ypos = display.getCursorY(); // позиция Y курсора при написании режима
// 	// display.println("Not");

// 	// display.print("Status: ");
// 	// int Stat_Xpos = display.getCursorX(); // позиция Х курсора при написании статуса
// 	// int Stat_Ypos = display.getCursorY(); // позиция Y курсора при написании статуса
// 	// display.println("Ground");

// 	// display.display();

// 	// MySerial3.println("AT+CIPCLOSE");
// 	// delay(500);
// 	// read_SIM868();

// 	// while (true)
// 	// {
// 	//   //  ========================= MODE AND SENDING ================================
// 	//   if ((millis() - start_time) >= 5000)
// 	//   {
// 	//     start_time = millis();
// 	//     MySerial1.println("==================================================================================");

// 	//     String data_transmitt = get_telemetry(Module_ADDR, status_count);

// 	//     if (digitalRead(STM_SW6) == false)
// 	//     { // ======================== MESH ============================
// 	//       draw_pos(Mode_Xpos, Mode_Ypos, "Mesh");
// 	//       send_to_mesh_E52(data_transmitt); // отправляем пакет (Если что то с модулем: data_transmitt = "GL 6666 56.452051 84.962577 174.967 1.5 190.4 1 2";)
// 	//     }

// 	//     else if (digitalRead(STM_SW6) == true)
// 	//     { // ======================== INTERNET ===========================
// 	//       draw_pos(Mode_Xpos, Mode_Ypos, "Internet");

// 	//       if (connect_flag == 0)
// 	//       {
// 	//         try_connect_to_server();
// 	//         connect_flag = check_connect_to_server();
// 	//       }

// 	//       if (connect_flag == 1)
// 	//       {
// 	//         send_to_server_SIM868(data_transmitt);
// 	//       }
// 	//     }
// 	//   }

// 	//   if (digitalRead(STM_BTN1) == false)
// 	//   { // ========================== SPEED/RANGE ======================================
// 	//     butt_count = Next_SR(butt_count, SR_Xpos, SR_Ypos);
// 	//   }

// 	//   if (digitalRead(LORA_RST) == false)
// 	//   { // ========================== STATUS ==========================
// 	//     status_count = Next_status(status_count, Stat_Xpos, Stat_Ypos);
// 	//   }

// 	//   if (digitalRead(STM_SW3) == true)
// 	//   { // ========================== POWER ==========================
// 	//     power_counter = Next_power(power_counter, Power_Xpos, Power_Ypos);
// 	//   }

// 	//   delay(500);
// 	// }
// }

#include "Arduino.h"

// Определение пинов для кнопок
const int buttonUpPin = PB0;
const int buttonDownPin = PB1;
const int buttonSelectPin = PA6;

// Структура для узла меню
enum MenuItemType
{
	MENU_ITEM,
	PARAMETER_ITEM
};

struct MenuItem
{
	const char *name;
	MenuItemType type;
	MenuItem *next;
	MenuItem *prev;
	MenuItem *submenu;	 // Указатель на подменю
	int *parameterValue; // Указатель на значение параметра (если это параметр)
};

// Класс для управления меню
class Menu
{
public:
	Menu() : head(nullptr), currentMenu(nullptr) {}

	MenuItem *addMenuItem(const char *name)
	{
		return addItem(name, MENU_ITEM, nullptr);
	}

	void addParameterItem(const char *name, int *parameterValue)
	{
		addItem(name, PARAMETER_ITEM, parameterValue);
	}

	void setSubmenu(MenuItem *item)
	{
		item->submenu = head; // Привязываем подменю к элементу
	}

	void navigate()
	{
		currentMenu = head;
		int selected = 0;

		while (true)
		{
			displayMenu(selected);

			if (digitalRead(buttonUpPin) == LOW)
			{
				currentMenu = (currentMenu->prev != nullptr) ? currentMenu = currentMenu->prev : currentMenu = tail;
				selected = (selected - 1 + countItems()) % countItems(); // Циклический выбор вверх
				delay(200);												 // Антидребезг
			}
			if (digitalRead(buttonDownPin) == LOW)
			{
				currentMenu = (currentMenu->next != nullptr) ? currentMenu = currentMenu->next : currentMenu = head;
				selected = (selected + 1) % countItems(); // Циклический выбор вниз
				delay(200);								  // Антидребезг
			}
			if (digitalRead(buttonSelectPin) == LOW)
			{
				if (currentMenu != nullptr && currentMenu->submenu != nullptr)
				{
					// Переход в подменю при выборе первого элемента
					navigateSubmenu(currentMenu->submenu);
				}
				else
				{
					handleSelection(selected);
				}
				delay(200); // Антидребезг
			}
		}
	}

private:
	MenuItem *head;
	MenuItem *tail;
	MenuItem *currentMenu;

	MenuItem *addItem(const char *name, MenuItemType type, int *parameterValue)
	{
		MenuItem *newItem = new MenuItem;
		newItem->name = name;
		newItem->type = type;
		newItem->next = nullptr;
		newItem->submenu = nullptr;				  // Изначально подменю пустое
		newItem->parameterValue = parameterValue; // Устанавливаем указатель на значение параметра

		if (head == nullptr)
		{
			newItem->prev = nullptr;
			head = newItem; // Если это первый элемент
			tail = newItem;
		}
		else
		{
			MenuItem *current = tail;

			current->next = newItem; // Добавляем новый элемент в конец списка
			newItem->prev = current; // Устанавливаем связь с предыдущим элементом

			tail = newItem;
		}
		return newItem;
	}

	void displayMenu(int selected)
	{
		display.clearDisplay();

		MenuItem *currentItem = head;
		int index = 0;

		while (currentItem != nullptr && index < 3)
		{ // Отображаем максимум 3 пункта меню
			if (index == selected)
			{
				display.setTextColor(BLACK, WHITE); // Инверсия цвета для выделенного пункта
			}
			else
			{
				display.setTextColor(WHITE);
			}
			display.setCursor(0, index * 10);
			display.println(currentItem->name);
			currentItem = currentItem->next;
			index++;
		}

		display.display();
	}

	void navigateSubmenu(MenuItem *submenu)
	{
		Menu subMenu;
		subMenu.head = submenu; // Устанавливаем голову подменю
		subMenu.navigate();		// Запускаем навигацию по подменю
	}

	void handleSelection(int selected)
	{
		MenuItem *currentItem = head;
		int index = 0;

		while (currentItem != nullptr && index < selected)
		{
			currentItem = currentItem->next;
			index++;
		}

		if (currentItem != nullptr && currentItem->type == PARAMETER_ITEM)
		{
			// Изменение значения параметра
			(*currentItem->parameterValue)++;
			displayParameterValue(currentItem);
		}
		else
		{
			// Здесь можно добавить логику выбора пункта меню
			// Например, выполнение действия для других пунктов меню
		}
	}

	void displayParameterValue(MenuItem *item)
	{
		display.clearDisplay();
		display.setCursor(0, 0);
		display.print(item->name);
		display.print(": ");
		display.println(*item->parameterValue);
		display.display();

		delay(2000); // Показываем значение параметра в течение 2 секунд
	}

	int countItems()
	{
		int count = 0;
		MenuItem *current = head;
		while (current != nullptr)
		{
			count++;
			current = current->next;
		}
		return count;
	}
};

Menu mainMenu;

int parameterValue1 = 0; // Пример значения параметра
int parameterValue2 = 5; // Пример другого значения параметра

void setup()
{
	Serial.begin(115200);
	display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.display();

	pinMode(buttonUpPin, INPUT_PULLUP);
	pinMode(buttonDownPin, INPUT_PULLUP);
	pinMode(buttonSelectPin, INPUT_PULLUP);

	// Пример добавления элементов в меню

	mainMenu.addParameterItem("Parameter 1", &parameterValue1);
	// Добавление параметров
	MenuItem *submenu10 = mainMenu.addMenuItem("Option 1");
	mainMenu.addParameterItem("Parameter 2", &parameterValue2);

	// Создание подменю и добавление пунктов
	Menu submenu;
	submenu.addMenuItem("Submenu 2");
	MenuItem *submenu2 = submenu.addMenuItem("Submenu 1");

	submenu.addParameterItem("Parameter 2", &parameterValue2);

	submenu.setSubmenu(submenu10); // Привязываем подменю к элементу

	Menu submenu1;
	submenu1.addMenuItem("Submenu 3");
	MenuItem *submenu3 = submenu1.addMenuItem("Submenu 4");

	submenu1.setSubmenu(submenu2);

	mainMenu.setSubmenu(submenu3); // Привязываем подменю к элементу

	mainMenu.navigate();
}

void loop()
{
	// Основной цикл может оставаться пустым, так как управление осуществляется в классе Menu
}
