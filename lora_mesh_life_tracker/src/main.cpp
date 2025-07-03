// // #include "functions.h"
#include "serial_init.h"
#include "SIM868.h"
#include "variables.h"
#include "e52.h"
#include "bmp280.h"
#include "pinout.h"
#include "screen.h"

SIM868 sim868(MySerial1, MySerial3);
// e52 my_e52(MySerial1, S_Serial);
bmp280 bmp;
screen my_screen(MySerial1);

void setup()
{ //========================== SETUP ===========================
	delay(5000);
	MySerial1.begin(115200); // обычный serial
	S_Serial.begin(115200);	 //
	MySerial3.begin(115200); // serial SIM868

	MySerial1.println("========= START SETUP =========");
	my_screen.begin();
	bmp.begin();
	// my_e52.E52_default_init();
	sim868.SIM868_Power_SW(SIM_PWRK);
	sim868.SIM868_GPS_Power_Up();
	sim868.setup_gprs_parameter();
	init_pinout();
	my_screen.cord = my_screen.init_menu(Module_ADDR);
	MySerial1.println("========= END SETUP =========");
}

void loop()
{ // ======================== LOOP ===============================
	if (millis() - timeout_altrate >= 1000)
	{ // определение скороподъемности и координат
		timeout_altrate = millis();

		altitude_rate = bmp.get_ar_with_filter(my_screen.cord.ALTR_Xpos, my_screen.cord.ALTR_Ypos);
		// MySerial1.print("alt_rate =");
		// MySerial1.println(altitude_rate);
		// router_hop = my_e52.read_router_hop();
		data_transmitt_old = sim868.get_telemetry(Module_ADDR, status_count, altitude_rate);
		MySerial1.print("data_T_old =");
		MySerial1.println(data_transmitt_old);

		if (data_transmitt == "")
		{
			data_transmitt = data_transmitt_old;
		}
		if (data_transmitt_old.indexOf("E") == -1)
		{ // отбраковываем данные которые содержат ешки
			data_transmitt = data_transmitt_old;
		}

		MySerial1.println(data_transmitt);
	}
	// #define HELL

	// #ifndef HELL
	//       Serial.println();
	// #endif
	//
	//     /*
	//     if(digitalRead(STM_SW2) == false)                        // SETTINGS
	//     {
	//       get_setup_from_ESP();                                 // получаем строку с настройками инастраиваемся
	//     }*/

	if ((millis() - start_time) >= 3000 + TX_timeout_random_piece) // режим отправки и отправка
	{
		start_time = millis();

		TX_timeout_random_piece = random(0, sim868.random_piece_upper_limit);

		MySerial1.print("Random = ");
		MySerial1.println(TX_timeout_random_piece);

		MySerial1.println("===========================================================================================");

		// String data_transmitt = get_telemetry(Module_ADDR, status_count, altitude_rate);

		//       // if (digitalRead(STM_SW6) == false)                         // MESH
		//       // {
		//       //   my_screen.draw_pos(cord.Mode_Xpos, cord.Mode_Ypos, "Mesh");
		//       //   counter_TX_pack++;
		//       //   send_to_mesh_E52("GL" + data_transmitt + " " + counter_TX_pack); // отправляем пакет (Если что то с модулем: data_transmitt = "GL 6666 56.452051 84.962577 174.967 1.5 190.4 1 2";)

		//       //   if (connect_flag == 0)                  // добавил дублирование отправкой на сервер, по просьбе организатора
		//       //   {
		//       //     try_connect_to_server();                            // пытаемя подключиьтся к серверу
		//       //     connect_flag = check_connect_to_server();           // проверяем получилось ли подключиться
		//       //   }

		//       //   if (connect_flag == 1)            // !!!!!!!!!0 потому что хочу посмотреть что пишет в таком случае должен быть 1!!!!!!!!
		//       //   {
		//       //     connect_flag = send_to_server_SIM868("GV" + data_transmitt + " " + counter_TX_pack);              // в случае дублирования ключ другой
		//       //   }
		//       // }

		//       // else
		if (digitalRead(STM_SW6) == true) // INTERNET
		{
			my_screen.draw_pos(my_screen.cord.Mode_Xpos, my_screen.cord.Mode_Ypos, "Internet");

			if (connect_flag == 0)
			{
				sim868.try_connect_to_server();					 // пытаемя подключиьтся к серверу
				connect_flag = sim868.check_connect_to_server(); // проверяем получилось ли подключиться
			}

			if (connect_flag == 1) // !!!!!!!!!0 потому что хочу посмотреть что пишет в таком случае должен быть 1!!!!!!!!
			{
				counter_TX_pack++;
				connect_flag = sim868.send_to_server_SIM868("GL" + data_transmitt + " " + counter_TX_pack); // если получилось подключиться то отправляем данные

				data_transmitt = "";
			}
		}
		//}

		state_SW1 = !digitalRead(STM_SW1);
		state_SW2 = !digitalRead(STM_SW2);
		state_SW3 = !digitalRead(STM_SW3);
		state_SW4 = !digitalRead(STM_SW4);
		state_SW5 = !digitalRead(STM_SW5);
		state_SW6 = !digitalRead(STM_SW6);

		//     if ((state_SW1 != old_state_SW1) or (state_SW2 != old_state_SW2))
		//     {
		//       if (state_SW1 + state_SW2 > 1) //  SPEED/RANGE
		//       {
		//         my_screen.draw_pos(cord.SR_Xpos, cord.SR_Ypos, "ERROR");
		//       }
		//       else if (state_SW1 == 1)
		//       {
		//         my_e52.set_rs(1);
		//         my_screen.draw_pos(cord.SR_Xpos, cord.SR_Ypos, "1");
		//       }
		//       else if (state_SW2 == 1)
		//       {
		//         my_e52.set_rs(2);
		//         my_screen.draw_pos(cord.SR_Xpos, cord.SR_Ypos, "2");
		//       }
		//       else if ((state_SW1 == 0) and (state_SW2 == 0))
		//       {
		//         my_e52.set_rs(0);
		//         my_screen.draw_pos(cord.SR_Xpos, cord.SR_Ypos, "0");
		//       }
		//       old_state_SW1 = state_SW1;
		//       old_state_SW2 = state_SW2;
		//     }

		if ((state_SW3 != old_state_SW3) or (state_SW4 != old_state_SW4))
		{
			if (state_SW3 + state_SW4 > 1)
			{
				my_screen.draw_pos(my_screen.cord.pause_Xpos, my_screen.cord.pause_Ypos, "ERROR");
			}
			else if (state_SW3 == 1)
			{
				sim868.random_piece_upper_limit = 7000;
				my_screen.draw_pos(my_screen.cord.pause_Xpos, my_screen.cord.pause_Ypos, "3-10");
			}
			else if (state_SW4 == 1)
			{
				sim868.random_piece_upper_limit = 12000;
				my_screen.draw_pos(my_screen.cord.pause_Xpos, my_screen.cord.pause_Ypos, "3-15");
			}
			else if ((state_SW3 == 0) and (state_SW4 == 0))
			{
				sim868.random_piece_upper_limit = 2000;
				my_screen.draw_pos(my_screen.cord.pause_Xpos, my_screen.cord.pause_Ypos, "3-5");
			}
			old_state_SW3 = state_SW3;
			old_state_SW4 = state_SW4;
		}

		// if (state_SW5 == 1)
		// {
		// 	my_e52.set_power(-8);
		// 	my_screen.draw_pos(cord.Power_Xpos, cord.Power_Ypos, "-8");
		// }
		// else
		// {
		// 	my_e52.set_power(22);
		// 	my_screen.draw_pos(cord.Power_Xpos, cord.Power_Ypos, "22");
		// }
	}
}