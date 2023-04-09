/*
 * buzzer.c
 *
 *  Created on: Nov 14, 2021
 *      Author: Dell
 */

#include "buzzer.h"
#include "gpio.h"

void buzzer_init(void){

	GPIO_setupPinDirection(buzzer_port, buzzer_pin1, PIN_OUTPUT);
	GPIO_setupPinDirection(buzzer_port, buzzer_pin2, PIN_OUTPUT);
	GPIO_writePin(buzzer_port, buzzer_pin1, LOGIC_LOW);
	GPIO_writePin(buzzer_port, buzzer_pin2, LOGIC_LOW);

}

void buzzer_start(void){
	GPIO_writePin(buzzer_port, buzzer_pin1, LOGIC_HIGH);
}


void buzzer_stop(void){
	GPIO_writePin(buzzer_port, buzzer_pin1, LOGIC_LOW);
}
