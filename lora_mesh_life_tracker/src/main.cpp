#include "serial_init.h"
#include "variables.h"
#include "pinout.h"
#include "logo.h"

#include "Screen.h"
#include "BMP280.h"
#include "SIM868.h"

SIM868 sim868(Terminal_UART, SIM868_UART);
BMP280 bmp;
Screen my_screen(Terminal_UART);

uint8_t MESH_STATUS_PIN = PB5;

void setup()
{
	// // delay(1000);				 //========================== SETUP ===========================
	Terminal_UART.begin(115200); // обычный serial
	SIM868_UART.begin(115200);	 // serial SIM868
	// LoRa_UART.begin(115200);

	init_pinout();

	my_screen.begin();
	// bmp.begin();

	// my_screen.drawBitmap(0, 0, PODNEBESE_LOGO, 128, 64, SSD1306_WHITE);

	// my_screen.fillRect(0, 50, 128, 16, SSD1306_BLACK);
	// my_screen.display();

	sim868.power_ON(SIM_PWRK);
	delay(15000);
	// sim868.PowerUp_gps();
	sim868.setup_gprs_parameter();

	// my_screen.fillRect(0, 50, 28, 8, SSD1306_WHITE);
	// my_screen.display();

	// float logoTimer = millis();
	// while (millis() - logoTimer < 15000)
	// {
	// 	float width = (((millis() - logoTimer) / 15000) * 100) + 28;
	// 	my_screen.fillRect(0, 50, width, 8, SSD1306_WHITE);
	// 	my_screen.display();
	// }
	// pinMode(MESH_STATUS_PIN, INPUT);
	// my_screen.cord = my_screen.draw_menu(module_ADDR);
	LoRa_UART.begin(115200);
}

void loop()
{
	Terminal_UART.print("marker");
	int timeout = millis();
	String Lora_data = "";
	String packet = "";
	int packetLength_counter = 0;
	// bool state_MESH;

	while (true)
	{

		/*
		if ((millis() - timeout) >= 1000)
		{
			LoRa_UART.println("GL 1234 90.000000 180.000000 165.165");
			timeout = millis();
			}*/

		Terminal_UART.print("marker2");

		Lora_data = LoRa_UART.readStringUntil('\n');

		Terminal_UART.print("readString =");
		Terminal_UART.println(Lora_data);
		// }

		if (Lora_data != "")
		{
			Terminal_UART.print("Lora_data =");
			Terminal_UART.println(Lora_data);

			packet = packet + " " + Lora_data + '\n';
			Lora_data = "";
			packetLength_counter++;

			Terminal_UART.print("packetLength_counter =");
			Terminal_UART.println(packetLength_counter);

			Terminal_UART.print("packet =");
			Terminal_UART.println(packet);
		}

		if (((millis() - start_time) >= 10000) or (packetLength_counter >= 10)) // режим отправки и отправка
		{
			start_time = millis();
			packetLength_counter = 0;

			// TX_timeout_random_piece = random(0, sim868.random_piece_upper_limit);

			// my_screen.draw_in_coordinates(my_screen.cord.Mode_Xpos, my_screen.cord.Mode_Ypos, "Internet");
			sim868.try_send_to_server(packet);

			packet = "";
		}

		// state_MESH = digitalRead(MESH_STATUS_PIN);
		// if (state_MESH == 0)
		// {
		// 	my_screen.draw_in_coordinates(0, 0, "DISCONNECT MESH");
		// }
		// if (state_MESH == 1)
		// {
		// 	my_screen.draw_in_coordinates(0, 0, "CONNECT MESH");
		// }
		// Terminal_UART.println("===========================================================================================");
	}
}