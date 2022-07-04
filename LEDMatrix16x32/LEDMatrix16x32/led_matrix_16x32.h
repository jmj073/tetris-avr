/*
 * led_matrix_16x32.h
 *
 * Created: 2022-06-29 15:48:06
 *  Author: JMJ073
 */ 


#ifndef LED_MATRIX_16X32_H_
#define LED_MATRIX_16X32_H_

#include <avr/io.h>
#include "shortint.h"
#include "my_util.h"

#define LEDMAT_H 16
#define LEDMAT_W 32

/* FOR RGB PINS */
#define LEDMAT_RGB		(&PINB)
#define LEDMAT_RGB_ALL	0x3F
#define LEDMAT_RGB1		0x07
#define LEDMAT_R1		0
#define LEDMAT_G1		1
#define LEDMAT_B1		2
#define LEDMAT_RGB2		(0x07 << 3)
#define LEDMAT_R2		3
#define LEDMAT_G2		4
#define LEDMAT_B2		5

/* FOR CONTROL PINS*/
#define LEDMAT_CR		(&PIND)
#define LEDMAT_CR_ALL	0x77
#define LEDMAT_A		0
#define LEDMAT_B		1
#define LEDMAT_C		2
//#define LEDMAT_D		3 // used on 32x32
#define LEDMAT_CLK		4
#define LEDMAT_LAT		5
#define LEDMAT_OE		6

#ifdef LED16X32_SHORT_MACRO

#define _R1		LEDMAT_R1
#define _G1		LEDMAT_G1
#define _B1		LEDMAT_B1
#define _R2		LEDMAT_R2
#define _G2		LEDMAT_G2
#define _B2		LEDMAT_B2
#define _A		LEDMAT_A
#define _B		LEDMAT_B
#define _C		LEDMAT_C
#define _CLK	LEDMAT_CLK
#define _LAT	LEDMAT_LAT
#define _OE		LEDMAT_OE

#define R1		_BV(LEDMAT_R1)
#define G1		_BV(LEDMAT_G1)
#define B1		_BV(LEDMAT_B1)
#define R2		_BV(LEDMAT_R2)
#define G2		_BV(LEDMAT_G2)
#define B2		_BV(LEDMAT_B2)
#define A		_BV(LEDMAT_A)
#define B		_BV(LEDMAT_B)
#define C		_BV(LEDMAT_C)
#define CLK		_BV(LEDMAT_CLK)
#define LAT		_BV(LEDMAT_LAT)
#define OE		_BV(LEDMAT_OE)

/* COLOR FLAG */
#define CR _BV(0)
#define CG _BV(1)
#define CB _BV(2)

/* functions */
#define LMAT_init		LEDMAT_init
#define LMAT_clear_all	LEDMAT_clear_all
#define LMAT_refresh	LEDMAT_refresh
#define LMAT_set_rgb	LEDMAT_set_rgb
#define LMAT_get_rgb	LEDMAT_get_rgb

#endif /* LED_MATRIX_16X32_SHORTCUT */

typedef u8 LEDMAT_t[LEDMAT_H / 2][LEDMAT_W];

#define DEF_LED_MATRIX(name) u8 name[LEDMAT_H / 2][LEDMAT_W]
#define DECL_LED_MATRIX(name) extern u8 name[LEDMAT_H / 2][LEDMAT_W]

void LEDMAT_init();
void LEDMAT_clear_all();
void LEDMAT_refresh();
static inline 
void LEDMAT_set_rgb(u8 r, u8 c, u8 rgb);
static inline 
u8 LEDMAT_get_rgb(u8 r, u8 c);


static inline 
void LEDMAT_set_rgb(u8 r, u8 c, u8 rgb)
{
	extern uint8_t __LED16X32_MATRIX[LEDMAT_H / 2][LEDMAT_W];
	
	if (r < 8) {
		__LED16X32_MATRIX[r][c] 
		= (__LED16X32_MATRIX[r][c] & ~LEDMAT_RGB1) 
		| (rgb & LEDMAT_RGB1);
	} else {
		__LED16X32_MATRIX[r & 7][c] 
		= (__LED16X32_MATRIX[r & 7][c] & ~LEDMAT_RGB2) 
		| ((rgb << 3) & LEDMAT_RGB2);
	}
}

static inline
u8 LEDMAT_get_rgb(u8 r, u8 c)
{
	extern uint8_t __LED16X32_MATRIX[LEDMAT_H / 2][LEDMAT_W];
	
	return r < 8 ?
		__LED16X32_MATRIX[r][c] & LEDMAT_RGB1:
		__LED16X32_MATRIX[r & 7][c] >> 3;
}


#endif /* LED_MATRIX_16X32_H_ */