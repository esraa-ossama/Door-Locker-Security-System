/*
 * buzzer.h
 *
 *  Created on: Nov 14, 2021
 *      Author: Dell
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include "std_types.h"
#include "gpio.h"


#define buzzer_pin1  PIN0_ID
#define buzzer_pin2  PIN1_ID
#define buzzer_port  PORTA_ID

void buzzer_init();
void buzzer_start();
void bzzer_stop();



#endif /* BUZZER_H_ */
