/*
 * Control-ECU.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Dell
 */

#include "timer.h"
#include "external_eeprom.h"
#include "uart.h"
#include "dc_motor.h"
#include "buzzer.h"

#define KEYPAD_DELAY 2500
#define UART_DELAY   130

volatile uint8 Valid;
volatile int quarter_sec = 0;



void RECEIVE_PW(uint8 pass[]) {
	uint8 i;
	for (i = 0; i < 4; i++) {
		pass[i] = UART_receiveByte();
	}
	_delay_ms(UART_DELAY);

}


void VERIFY_PW(uint8 Pass[], uint8 check_pass[]) {
	uint8 i;
	for (i = 0; i < 4; i++) {
		if (Pass[i] != check_pass[i])//IF ONE CHAR IS DIFFRENT THEN PW IS INVALID
				{
			UART_sendByte(0);
			_delay_ms(UART_DELAY);
			Valid = 0;
			break;
		}
		Valid = 1;
	}

	if (Valid) {
		UART_sendByte(1);
		_delay_ms(UART_DELAY);
	}
}


void timer0_isr_fn(void) {
	quarter_sec++;
	if (quarter_sec == 30) {
		SECONDS_T0_MC1++;
		SECONDS_T0_MC2++;
		quarter_sec = 0;
	}
}


int main(void) {
	uint8 j;
	uint8 k;
	uint8 P_W[4];
	uint8 check[4];
	Timer0_setCallBack(timer0_isr_fn);
	buzzer_init();
	EEPROM_init();
	MOTOR_init();
	UART_Config UConfig = { ASYNCH, ENABLED_EVEN, BIT_1 };
	UART_init(&UConfig);

	TimerConfig T0_Configuration = { NORMAL, F_CPU_1024, 0, 7813 };
	Timer0_init(&T0_Configuration); 			 //Initialize timer0
	SREG |= (1 << 7);

	// get pass and check it
	do {
		RECEIVE_PW(P_W);        	// RECIEVE FIRST PW USER SENDS
		RECEIVE_PW(check);        // RECIEVE VERIFYING PW USER SENDS
		VERIFY_PW(P_W, check);		//CHECK IF PW'S SENT FROM THE HMI MATCH
	} while (Valid == 0);

	while (1) {
		uint8 command = UART_receiveByte();
		if (command == '-') {
			do {
				RECEIVE_PW(P_W);        	// RECIEVE FIRST PW USER SENDS
				RECEIVE_PW(check);        // RECIEVE VERIFYING PW USER SENDS
				VERIFY_PW(P_W, check);	//CHECK IF PW'S SENT FROM THE HMI MATCH
			} while (Valid == 0);
		}

		// SAVE PASS IN EEPROM
		for (j = 0; j < 4; j++) {
			EEPROM_writeByte((0X0090 + j), P_W[j]);
			_delay_ms(UART_DELAY);
		}
		if (command == '+') {

			/* GET PASSWORD IN EEPROM AND SAVE IT IN A VARIABLE TO CHECK PW USER SENT */
			for (k = 0; k < 4; k++) {
				EEPROM_readByte((0x0090 + k), (P_W + k));
				_delay_ms(100);
			}
			// the user 3 chance for password
			uint8 count = 0;
			do {
				count++;
				RECEIVE_PW(check);     // RECIEVE PW USER SENDS WE WANT TO CHECK
				VERIFY_PW(P_W, check);		//CHECK IF PW USER SENT IS CORRECT
			} while (Valid == 0 && count < 3);

			if (Valid) {
				SECONDS_T0_MC2 = 0;
				while (SECONDS_T0_MC2 <= 15) {
					MOTOR_clockw
					;
				}
				while (SECONDS_T0_MC2 <= 18) {
					MOTOR_stop
					;
				}
				while (SECONDS_T0_MC2 <= 33) {
					MOTOR_anti_clockw
					;
				}
				MOTOR_stop
				;
			} else if (!Valid) {
				buzzer_start();
				SECONDS_T0_MC2 = 0;
				while (SECONDS_T0_MC2 <= 60)
					;
				buzzer_stop();
				break;
			}
		}

	}

}

