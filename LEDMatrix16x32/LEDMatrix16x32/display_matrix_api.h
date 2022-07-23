/*
 * display_matrix_api.h
 *
 * Created: 2022-07-06 12:03:56
 *  Author: user
 */ 


#ifndef DISPLAY_MATRIX_API_H_
#define DISPLAY_MATRIX_API_H_

#include "led_matrix_16x32.h"

/* type for coordinates */
typedef u8 coord_t;

#define DMAT_ROW	LEDMAT_COL
#define DMAT_COL	LEDMAT_ROW 

#define DMAT_swap_buffer() LEDMAT_swap_buffer()

/* COLOR FLAG */
#define CR _BV(0)
#define CG _BV(1)
#define CB _BV(2)
#define DMAT_set_rgb_bit(row, col, rgb)\
		LEDMAT_set_rgb_bit((LEDMAT_ROW - 1) - (col), (row), (rgb))

#define DMAT_set_rgb() ({ _Static_assert(0,\
		"DMAT_set_rgb is not implemented."); })

#define DMAT_clear() LEDMAT_clear()

#define DMAT_start_write() ((void)0)

// no copy(copy Front Buffer to Back Buffer)
#define DMAT_NCP	_BV(0)
// Back Buffer clear
#define DMAT_CLR	_BV(1)
static inline void DMAT_end_write(u8 flags) {
	DMAT_swap_buffer(); // back buffer <=> front buffer
	
	if (flags & DMAT_CLR)
		DMAT_clear(); // back buffer clear
	else if (~flags & DMAT_NCP)
		LEDMAT_copy_buffer(); // copy front buffer to back buffer
}

void DMAT_draw_rect_fill(coord_t r, coord_t c, coord_t h, coord_t w, u8 rgb);
void DMAT_draw_square_fill(coord_t r, coord_t c, coord_t side_size, u8 rgb);

#define DMAT_DIGIT_RATIO_H 5
#define DMAT_DIGIT_RATIO_W 3
// scale = 1, 2, 3, ...
void DMAT_draw_digit_bit(coord_t row, coord_t col, u8 n, u8 rgb, coord_t scale);


#endif /* DISPLAY_MATRIX_API_H_ */