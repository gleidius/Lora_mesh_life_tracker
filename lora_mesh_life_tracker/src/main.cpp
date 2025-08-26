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

	Terminal_UART.println("START_SETTINGS");
	init_pinout();

	my_screen.begin();

	sim868.power_ON(SIM_PWRK);
	delay(15000);
	sim868.PowerUp_gps();
	delay(2000);
	sim868.setup_gprs_parameter();

	sim868.send_AT_command(("AT+CFUN=1"));

	// sim868.send_AT_command(("AT+CFUN=4"));
	// delay(1000);
	sim868.send_AT_command(("AT+BTPOWER=0"));
	delay(1000);
	sim868.send_AT_command(("AT+CGNSPWR=1"));
	delay(1000);
	sim868.send_AT_command(("AT+CGNSPWR=1"));
	delay(1000);
	sim868.send_AT_command(("AT+CGNSCMD=0,\"$PMTK353,1,1,1,0,0*2A\""));
	delay(1000);
	sim868.send_AT_command(("AT+CGNSTST=1"));
	delay(1000);
	sim868.send_AT_command(("AT+CGNSCMD=0,\"$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*34\""));
	delay(1000);

	// sim868.setup_gprs_parameter();

	LoRa_UART.begin(115200);
}

void loop()
{
	// Terminal_UART.print("marker");
	int timeout = millis();
	int timeout_sending_length = 10000; // таймаут в миллисекундах по прошествии которого данные отправляются на сервер независимо от их количества
	bool first_read_past_send = 0;		// флажок необходимый для определения первой вычитки юарт, после отправки данных на сервер

	int max_pack_length = 90;	 // максимальная длинна одного пакета
	int max_pack_in_string = 10; // максимальное количество пакетов которое будет упаковано в один для отправки на сервер
	String Lora_data = "";
	Lora_data.reserve(max_pack_length);
	String packet = "";
	packet.reserve(max_pack_in_string * max_pack_length);
	int packetLength_counter = 0;
	// my_screen.begin();
	//  bool state_MESH;

	while (true)
	{

		/*
		if ((millis() - timeout) >= 1000)
		{
			LoRa_UART.println("GL 1234 90.000000 180.000000 165.165");
			timeout = millis();
			}*/

		// Terminal_UART.print("marker2");

		// while (true)
		//{
		Lora_data = LoRa_UART.readStringUntil('\n');
		// Lora_data = LoRa_UART.readString();

		Terminal_UART.print("readString =");
		Terminal_UART.println(Lora_data);
		//}

		// }

		if (Lora_data != "")
		{
			if (Lora_data.substring(2).indexOf("GL") == -1)
			{

				Terminal_UART.print("Lora_data =");
				Terminal_UART.println(Lora_data);

				// packet += packet; // подобная конструкция необходима для экономии времени при сборке строки
				// packet += " ";
				// packet += Lora_data;
				// packet += '\n';
				packet = packet + " " + Lora_data + '\n';

				Lora_data = "";
				packetLength_counter++;

				Terminal_UART.print("packetLength_counter =");
				Terminal_UART.println(packetLength_counter);

				Terminal_UART.print("packet =");
				Terminal_UART.println(packet);

				// Terminal_UART.print("Point 1");
			}
			else
			{
				Lora_data = "";
			}
		}

		if (((millis() - start_time) >= timeout_sending_length) or (packetLength_counter >= max_pack_in_string)) // режим отправки и отправка
		{
			sim868.send_AT_command(("AT+CGNSTST=0"));
			start_time = millis();
			packetLength_counter = 0;

			// packet = "GL 1234 90.000000 180.000000 165.165"; //!!!!!!!!!! заглушка для тестов !!!!!!!!!

			sim868.try_send_to_server(packet);

			packet = "";

			my_screen.drawSignalIcon(sim868.readBaseStationPowerImage());
			sim868.send_AT_command(("AT+CGNSTST=1"));
			// my_screen.drawSignalIcon(sim868.readBaseStationPowerImage());
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