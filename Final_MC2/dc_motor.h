/*
 * dc_motor.h
 *
 *  Created on: Nov 14, 2021
 *      Author: Dell
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "gpio.h"
#include "common_macros.h"
#include "std_types.h"
#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


#define A PC3
#define B PC4
#define MOTOR_PORT_DIR DDRC
#define MOTOR_PORT PORTC

/* MOTOR Commands */
#define MOTOR_stop	 		CLEAR_BIT(MOTOR_PORT,A);  CLEAR_BIT(MOTOR_PORT,B)
#define MOTOR_anti_clockw 	CLEAR_BIT(MOTOR_PORT,A);  SET_BIT(MOTOR_PORT,B)
#define MOTOR_clockw		SET_BIT(MOTOR_PORT,A);	  CLEAR_BIT(MOTOR_PORT,B)

void MOTOR_init(void);

extern volatile uint8 g_MOTOR_flag;


#endif /* DC_MOTOR_H_ */
