/*
 * timer.h
 *
 *  Created on: Nov 14, 2021
 *      Author: Dell
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include "gpio.h"
#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

extern volatile uint32 SECONDS_T0_MC1;
extern volatile uint32 SECONDS_T0_MC2;

typedef enum
{
	NORMAL=0x80, PWM=0x40 ,CTC=0x88 , FAST_PWM=0x48
}Timer_Mode;


typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef struct
{
	Timer_Mode mode;
	Timer_Clock clock;
	uint16 initial_value;
	uint16 compare_value;
}TimerConfig;


void Timer0_init(const TimerConfig  *configuration);
void Timer0_deInit();
void Timer0_stop();
void Timer0_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_H_ */
