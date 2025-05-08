#include "functions.h"
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

// 	// // инициализируем софтовые/хардовые serial-ы
// 	// MySerial1.begin(115200); // обычный serial
// 	S_Serial.begin(115200);
// 	Serial.begin(115200);
// 	// MySerial3.begin(115200); // serial SIM868

// 	delay(5000); ///////// нужен чтобы успеть открыть монитор порта потом удалить!!!!!!

// 	meshApp.setup(&Serial);
// 	meshApp.sendMessage("Start STM32");

// 	e52.setup(&S_Serial);
// 	meshApp.sendMessage("Init E52");

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

// 	//   delay(500);
// 	// }
// }

uint16_t parameterValue1 = 2; // Пример значения параметра
uint16_t parameterValue2 = 5; // Пример другого значения параметра

Button buttonOk(buttonSelectPin);
Button buttonDown(buttonDownPin);
Button buttonUp(buttonUpPin);

void setup()
{
	Serial.begin(115200);
	delay(5000);
	Serial.println("Start STM32");

	menu.setup(&buttonOk, &buttonUp, &buttonDown);

	Serial.println("end init");
}

void loop()
{
	menu.navigate();
	delay(100);
	Serial.println(buttonUpPin);
}
