#include "serial_init.h"
#include "variables.h"
#include "pinout.h"
#include "logo.h"

#include "Screen.h"
#include "SIM868.h"

// #define MESH_STATUS_PIN PB7

uint8_t MESH_STATUS_PIN = PB9;

SIM868 sim868(Terminal_UART, SIM868_UART);
Screen my_screen(Terminal_UART);

void setup()
{
	// delay(1000);				 //========================== SETUP ===========================
	Terminal_UART.begin(115200); // обычный serial
	SIM868_UART.begin(115200);	 // serial SIM868

	my_screen.begin();

	my_screen.drawBitmap(0, 0, PODNEBESE_LOGO, 128, 64, SSD1306_WHITE);

	sim868.power_ON(SIM_PWRK);
	// sim868.PowerUp_gps();
	// delay(2000);
	// sim868.send_AT_command("AT+CGNSCMD=0,\"$PMTK353,1,1,1,0,0*2A\"");

	my_screen.fillRect(0, 0, 128, 64, SSD1306_BLACK);
	my_screen.display();
	pinMode(MESH_STATUS_PIN, INPUT_PULLDOWN);
}
bool state_MESH;

void loop()
{
	// Terminal_UART.println("loop");
	state_MESH = digitalRead(MESH_STATUS_PIN);

	Terminal_UART.print(state_MESH);
	Terminal_UART.println(state_MESH);

	if (state_MESH == 0)
	{
		my_screen.draw_in_coordinates(0, 0, "DISCONNECT MESH");
	}
	if (state_MESH == 1)
	{
		my_screen.draw_in_coordinates(0, 0, "CONNECT MESH");
	}
}
