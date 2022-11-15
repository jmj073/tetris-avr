/*
 * display_matrix_api.c
 *
 * Created: 2022-07-07 22:31:39
 *  Author: JMJ
 */ 

#include "display_matrix_api.h"

static const u16 DIGITS[10] = {
	0x7B6F, 0x2492, 0x73E7, 0x79E7, 0x49ED, 0x79CF, 0x7BCF, 0x492F, 0x7BEF, 0x79EF
};// 1 우측정렬: 0x4924, 1 중앙 정렬: 0x2492

static inline void __DMAT_draw_rect_fill(coord_t r, coord_t c, coord_t h, coord_t w, u8 rgb)
{
	h += r;
	w += c;
	
	coord_t col = c;

	for (; r < h; r++) {
		for (c = col;c < w; c++) {
			DMAT_set_rgb_bit(r, c, rgb);
		}
	}
}

void DMAT_draw_rect_fill(coord_t r, coord_t c, coord_t h, coord_t w, u8 rgb)
{
	__DMAT_draw_rect_fill(r, c, h, w, rgb);
}

void DMAT_draw_square_fill(coord_t r, coord_t c, coord_t side_size, u8 rgb)
{
	__DMAT_draw_rect_fill(r, c, side_size, side_size, rgb);
}

void DMAT_draw_digit_bit(coord_t row, coord_t col, u8 n, u8 rgb, coord_t scale)
{
	u16 digit = DIGITS[n];

	for (coord_t r = 0; r < DMAT_DIGIT_RATIO_H; r++) {
		for (coord_t c = 0; c < DMAT_DIGIT_RATIO_W; c++) {
			if (digit & 1) {
				DMAT_draw_square_fill(r * scale + row, c * scale + col, scale, rgb);
			}
			digit >>= 1;
		}
	}
}

#ifdef DISPLAY_MATRIX_EEPROM_ENABLE
#include <avr/eeprom.h>

void DMAT_draw_screen_from_eeprom(const u8* addr)
{
	for (coord_t r = 0; r < DMAT_ROW; r++) {
		for (coord_t c = 0; c < DMAT_COL / 2; c++) {
			u8 rgb = eeprom_read_byte(addr++);
			DMAT_set_rgb_bit(r, c, rgb & 7);
			DMAT_set_rgb_bit(r, c + DMAT_COL / 2, rgb >> 3);
		}
	}
}
 
#endif /* eeprom */

#ifdef DISPLAY_MATRIX_PGM_ENABLE
#include <avr/pgmspace.h>

void DMAT_draw_screen_from_pgm(const u8* addr)
{
	for (coord_t r = 0; r < DMAT_ROW; r++) {
		for (coord_t c = 0; c < DMAT_COL / 2; c++) {
			u8 rgb = pgm_read_byte(addr++);
			DMAT_set_rgb_bit(r, c, rgb & 7);
			DMAT_set_rgb_bit(r, c + DMAT_COL / 2, rgb >> 3);
		}
	}
}

#endif /* pgm */