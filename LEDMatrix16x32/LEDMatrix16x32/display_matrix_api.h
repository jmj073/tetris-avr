/*
 * display_matrix_api.h
 *
 * Created: 2022-07-06 12:03:56
 *  Author: user
 */ 


#ifndef DISPLAY_MATRIX_API_H_
#define DISPLAY_MATRIX_API_H_

#include "led_matrix_16x32.h"

//#define DISPLAY_MATRIX_EEPROM_ENABLE
#define DISPLAY_MATRIX_PGM_ENABLE

/* type for coordinates */
typedef u8 coord_t;

#define DMAT_ROW	LEDMAT_COL
#define DMAT_COL	LEDMAT_ROW


#define DMAT_clear() LEDMAT_fill_rgb_bit(0)
#define DMAT_fill_bit(rgb) LEDMAT_fill_rgb_bit(rgb)


/* not implemented */
//#define DMAT_set_rgb()

/* COLOR FLAG */
#define CR _BV(0)
#define CG _BV(1)
#define CB _BV(2)
static inline void DMAT_set_rgb_bit(u8 row, u8 col, u8 rgb) {
#if 0 /* range check */
	if (row >= DMAT_ROW || col >= DMAT_COL) {
		DMAT_fill_bit(CR);
		LEDMAT_swap_buffer();
		while (1);
	}
#endif
	
	LEDMAT_set_rgb_bit((LEDMAT_ROW - 1) - col, row, rgb);
}


#define DMAT_CP	_BV(0) // copy(copy Front Buffer to Back Buffer)
static inline void DMAT_update(u8 flags) {
	LEDMAT_swap_buffer(); // back buffer <=> front buffer
	
	if (flags & DMAT_CP)
		LEDMAT_copy_buffer(); // copy front buffer to back buffer
}



void DMAT_draw_rect_fill(coord_t r, coord_t c, coord_t h, coord_t w, u8 rgb);
void DMAT_draw_square_fill(coord_t r, coord_t c, coord_t side_size, u8 rgb);

// 0 < scale
#define DMAT_DIGIT_RATIO_H 5
#define DMAT_DIGIT_RATIO_W 3
void DMAT_draw_digit_bit(coord_t row, coord_t col, u8 n, u8 rgb, coord_t scale);

#ifdef DISPLAY_MATRIX_EEPROM_ENABLE
void DMAT_draw_screen_from_eeprom(const u8* addr);
#endif

#ifdef DISPLAY_MATRIX_PGM_ENABLE
void DMAT_draw_screen_from_pgm(const u8* addr);
#endif

#endif /* DISPLAY_MATRIX_API_H_ */