/*
 * display_matrix_api.h
 *
 * Created: 2022-07-06 12:03:56
 *  Author: user
 */ 


#ifndef DISPLAY_MATRIX_API_H_
#define DISPLAY_MATRIX_API_H_

#include "led_matrix_16x32.h"

// 32
#define DMAT_ROW	LEDMAT_COL
// 16
#define DMAT_COL	LEDMAT_ROW 

#define DMAT_init() LEDMAT_init()

#define DMAT_swap_buffer() LEDMAT_swap_buffer()

/* COLOR FLAG */
#define CR _BV(0)
#define CG _BV(1)
#define CB _BV(2)

#define DMAT_set_rgb_bit(row, col, rgb)\
		LEDMAT_set_rgb_bit((LEDMAT_ROW - 1) - (col), (row), (rgb))
#define DMAT_set_rgb() ({ _Static_assert(0,\
		"DMAT_set_rgb_bit is not implemented."); })
#define DMAT_clear() LEDMAT_clear()

#define DMAT_start_write() ((void)0)


// non copy(copy Front Buffer to Back Buffer)
#define DMAT_NCOPY	_BV(0)
// Back Buffer clear
#define DMAT_CLEAR	_BV(1)
static inline void DMAT_end_write(u8 flags) {
	DMAT_swap_buffer();
	
	if (flags & DMAT_CLEAR)
		DMAT_clear();
	else if (~flags & DMAT_NCOPY) {
		LEDMAT_copy_buffer();
	}
}


#endif /* DISPLAY_MATRIX_API_H_ */