/*
 * interrupts.c
 *
 * Created: 2022-07-03 15:04:51
 *  Author: JMJ
 */ 

#include <avr/interrupt.h>
#include "pins.h"
#include "task_queue.h"
#include "shortint.h"
#define LED16X32_SHORT_MACRO
#include "led_matrix_16x32.h"

/* for LED matrix refresh */
ISR(TIMER2_OVF_vect) {
	LMAT_refresh();
}