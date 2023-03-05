/*
 * led_matrix_16x32.h
 *
 * Created: 2022-06-29 15:48:06
 *  Author: JMJ073
 */ 


#ifndef LED_MATRIX_16X32_H_
#define LED_MATRIX_16X32_H_

#include <avr/io.h>
#include <string.h>
#include "shortint.h"
#include "my_util.h"

#define LEDMAT_ROW 16U
#define LEDMAT_COL 32U

#define LEDMAT_SECTION (LEDMAT_ROW / 2)

/* RGB PINS */
#define LEDMAT_RGB		(&PINA)
#define LEDMAT_RGB_ALL	0x3F
#define LEDMAT_RGB1		0x07
#define LEDMAT_R1		0
#define LEDMAT_G1		1
#define LEDMAT_B1		2
#define LEDMAT_RGB2		(0x07 << 3)
#define LEDMAT_R2		3
#define LEDMAT_G2		4
#define LEDMAT_B2		5

/* CONTROL PINS*/
#define LEDMAT_CR		(&PINB)
#define LEDMAT_CR_ALL	0xB7
#define LEDMAT_A		0
#define LEDMAT_B		1
#define LEDMAT_C		2
//#define LEDMAT_D		3 // used on 32x32
#define LEDMAT_CLK		4
#define LEDMAT_LAT		5
#define LEDMAT_OE		7

#define LEDMAT_MIN_BRIGHTNESS		((u8)0)
#define LEDMAT_MAX_BRIGHTNESS		((u8)-1)
#define LEDMAT_DEFAULT_BRIGHTNESS	((u8)64)

void LEDMAT_init();

void LEDMAT_swap_buffer();
void LEDMAT_copy_buffer();

void LEDMAT_set_rgb_bit(u8 r, u8 c, u8 rgb);
u8   LEDMAT_get_rgb_bit(u8 r, u8 c);
void LEDMAT_fill_rgb_bit(u8 rgb);

void LEDMAT_set_brightness(u8 brightness);
u8   LEDMAT_get_brightness();

#endif /* LED_MATRIX_16X32_H_ */