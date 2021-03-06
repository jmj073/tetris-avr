/*
 * interrupts.c
 *
 * Created: 2022-07-03 15:04:51
 *  Author: JMJ
 */ 

#include <avr/interrupt.h>
#include "pins.h"
#include "shortint.h"
#include "led_matrix_16x32.h"

/* for LED matrix refresh */
ISR(TIMER2_OVF_vect) {
	LEDMAT_refresh();
}

/* brightness control */
ISR(TIMER2_COMP_vect) {
	LEDMAT_OE_disable();
}