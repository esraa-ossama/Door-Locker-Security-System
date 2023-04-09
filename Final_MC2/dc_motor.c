/*
 * dc_motor.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Dell
 */
#include "dc_motor.h"


//GPIO_setupPinDirection(PIN3_ID, PORTB_ID, PIN_OUTPUT);

void MOTOR_init(void)
{
	/* A , B Outputs of MICROCONTROLLER */
	SET_BIT(MOTOR_PORT_DIR,A);
	SET_BIT(MOTOR_PORT_DIR,B);
}

