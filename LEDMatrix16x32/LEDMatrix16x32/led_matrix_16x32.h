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

#define LEDMAT_ROW 16
#define LEDMAT_COL 32

#define LEDMAT_SECTION (LEDMAT_ROW / 2)

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
#define LMAT_init			LEDMAT_init
#define LMAT_clear_all		LEDMAT_clear
#define LMAT_refresh		LEDMAT_refresh
#define LMAT_set_rgb_bit	LEDMAT_set_rgb_bit
#define LMAT_get_rgb_bit	LEDMAT_get_rgb_bit

#endif /* LED_MATRIX_16X32_SHORTCUT */

// 임의로 수정하지 말것
extern u8 __LEDMAT_BUFFER[2][LEDMAT_SECTION][LEDMAT_COL];
extern u8 __LEDMAT_CURR_BUF;
#define LEDMAT_FRONT_BUF (__LEDMAT_BUFFER[__LEDMAT_CURR_BUF])
#define LEDMAT_BACK_BUF (__LEDMAT_BUFFER[__LEDMAT_CURR_BUF ^ 1])

void LEDMAT_init();

static inline void LEDMAT_swap_buffer();
static inline void LEDMAT_copy_buffer();

static inline void LEDMAT_set_rgb_bit(u8 r, u8 c, u8 rgb);
static inline u8 LEDMAT_get_rgb_bit(u8 r, u8 c);
void LEDMAT_clear();

void LEDMAT_refresh();


static inline void LEDMAT_swap_buffer()
{
	__LEDMAT_CURR_BUF ^= 1;
}
static inline void LEDMAT_copy_buffer()
{
	memcpy(LEDMAT_BACK_BUF, LEDMAT_FRONT_BUF, sizeof(LEDMAT_BACK_BUF));
}

static inline void LEDMAT_set_rgb_bit(u8 r, u8 c, u8 rgb)
{
	if (r < 8) {
		LEDMAT_BACK_BUF[r][c] 
		= (LEDMAT_BACK_BUF[r][c] & ~LEDMAT_RGB1) 
		| (rgb & LEDMAT_RGB1);
	} else {
		LEDMAT_BACK_BUF[r & 7][c] 
		= (LEDMAT_BACK_BUF[r & 7][c] & ~LEDMAT_RGB2) 
		| ((rgb << 3) & LEDMAT_RGB2);
	}
}
static inline u8 LEDMAT_get_rgb_bit(u8 r, u8 c)
{
	return r < 8 ?
		LEDMAT_BACK_BUF[r][c] & LEDMAT_RGB1:
		LEDMAT_BACK_BUF[r & 7][c] >> 3;
}


#endif /* LED_MATRIX_16X32_H_ */