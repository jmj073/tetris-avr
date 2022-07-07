/*
 * display_matrix_api.c
 *
 * Created: 2022-07-07 22:31:39
 *  Author: JMJ
 */ 

#include "display_matrix_api.h"

static const u16 DIGITS[10] = {
	0x7B6F, 0x4924, 0x73E7, 0x79E7, 0x49ED, 0x79CF, 0x7BCF, 0x492F, 0x7BEF, 0x79EF
};

void DMAT_draw_digit_bit(u8 row, u8 col, u8 n, u8 rgb) {
	u16 digit = DIGITS[n];

	for (u8 r = row; r < row + 5; r++)
	for (u8 c = col; c < col + 3; c++) {
		if (digit & 1) DMAT_set_rgb_bit(r, c, rgb);
		digit >>= 1;
	}
}