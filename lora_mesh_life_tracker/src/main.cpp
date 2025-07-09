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

void setup()
{
	//delay(1000);				 //========================== SETUP ===========================
	Terminal_UART.begin(115200); // обычный serial
	LoRa_UART.begin(115200);
	SIM868_UART.begin(115200); // serial SIM868

	init_pinout();

	my_screen.begin();
	bmp.begin();

	my_screen.drawBitmap(0, 0, PODNEBESE_LOGO, 128, 64, SSD1306_WHITE);

	my_screen.fillRect(0, 50, 128, 16, SSD1306_BLACK);
	my_screen.display();

	sim868.power_ON(SIM_PWRK);
	sim868.PowerUp_gps();
	sim868.setup_gprs_parameter();

	my_screen.fillRect(0, 50, 28, 8, SSD1306_WHITE);
	my_screen.display();

	float logoTimer = millis();
	while (millis() - logoTimer < 15000)
	{
		float width = (((millis() - logoTimer) / 15000) * 100) + 28;
		my_screen.fillRect(0, 50, width, 8, SSD1306_WHITE);
		my_screen.display();
	}

	my_screen.cord = my_screen.draw_menu(module_ADDR);
}

void loop()
{
	Terminal_UART.println("===========================================================================================");

	Terminal_UART.println("ii"); // ======================== LOOP ===============================
	if (millis() - timeout_altrate >= 1000)
	{ // определение скороподъемности и координат
		timeout_altrate = millis();

		altitude_rate = bmp.get_ar_with_filter();
		sim868.get_telemetry(module_ADDR, status_count, altitude_rate); //!!!!!!!!!!!!1
		sim868.filter_incorrect_data();

		my_screen.drawSignalIcon(sim868.readBaseStationPowerImage());
	}

	if ((millis() - start_time) >= 3000 + TX_timeout_random_piece) // режим отправки и отправка
	{
		start_time = millis();
		TX_timeout_random_piece = random(0, sim868.random_piece_upper_limit);

		if (digitalRead(STM_SW6) == false) // ==================== MESH ========================
		{
			my_screen.draw_in_coordinates(my_screen.cord.Mode_Xpos, my_screen.cord.Mode_Ypos, "Mesh");
			LoRa_UART.println(sim868.get_data_transmitt());
			Terminal_UART.print("Lora_Pack =");
			Terminal_UART.println(sim868.get_data_transmitt());
			sim868.try_send_to_server();
		}

		if (digitalRead(STM_SW6) == true) // INTERNET
		{
			my_screen.draw_in_coordinates(my_screen.cord.Mode_Xpos, my_screen.cord.Mode_Ypos, "Internet");
			sim868.try_send_to_server();
		}
	}

	state_SW1 = !digitalRead(STM_SW1);
	state_SW2 = !digitalRead(STM_SW2);
	state_SW3 = !digitalRead(STM_SW3);
	state_SW4 = !digitalRead(STM_SW4);
	state_SW5 = !digitalRead(STM_SW5);
	state_SW6 = !digitalRead(STM_SW6);

	if ((state_SW3 != old_state_SW3) or (state_SW4 != old_state_SW4))
	{
		if (state_SW3 + state_SW4 > 1)
		{
			my_screen.draw_in_coordinates(my_screen.cord.pause_Xpos, my_screen.cord.pause_Ypos, "ERROR");
		}
		else if (state_SW3 == 1)
		{
			sim868.random_piece_upper_limit = 7000;
			my_screen.draw_in_coordinates(my_screen.cord.pause_Xpos, my_screen.cord.pause_Ypos, "3-10");
		}
		else if (state_SW4 == 1)
		{
			sim868.random_piece_upper_limit = 12000;
			my_screen.draw_in_coordinates(my_screen.cord.pause_Xpos, my_screen.cord.pause_Ypos, "3-15");
		}
		else if ((state_SW3 == 0) and (state_SW4 == 0))
		{
			sim868.random_piece_upper_limit = 2000;
			my_screen.draw_in_coordinates(my_screen.cord.pause_Xpos, my_screen.cord.pause_Ypos, "3-5");
		}
		old_state_SW3 = state_SW3;
		old_state_SW4 = state_SW4;
	}
}
