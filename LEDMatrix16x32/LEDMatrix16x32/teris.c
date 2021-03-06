/*
 * teris.c
 *
 * Created: 2022-07-05 22:09:27
 *  Author: JMJ
 */ 

#include <stdlib.h>

#include "tetris.h"
#include "pins.h"
#include "settings.h"
#include "timer.h"

#define UP		_BV(BtN_UP)
#define DOWN	_BV(BtN_DOWN)
#define LEFT	_BV(BtN_LEFT)
#define RIGHT	_BV(BtN_RIGHT)

// block layout is: {w-1,h-1}{x0,y0}{x1,y1}{x2,y2}{x3,y3} (two bits each, total 20 bits)

#define BOARD_ROW	20
#define BOARD_COL	10

static u8 X, Y, R, PX, PY, PR, P;
static u16 SCORE = 0;
static u8 BOARD[BOARD_ROW][BOARD_COL];

static const u32 BLOCK[7][4] = {
	{ 431424, 598356, 431424, 598356 },
	{ 427089, 615696, 427089, 615696 },
	{ 348480, 348480, 348480, 348480 },
	{ 599636, 431376, 598336, 432192 },
	{ 411985, 610832, 415808, 595540 },
	{ 247872, 799248, 247872, 799248 },
	{ 614928, 399424, 615744, 428369 },
};

// extract a 2-bit number from a block entry
static inline i8 NUM(i8 r, i8 n) { return (BLOCK[P][r] >> n) & 3; }

// create a new piece, don't remove old one (it has landed and should stick)
static void new_piece() {
	int rnum = rand();
	Y = PY = 0;
	P = rnum % 7;
	R = PR = rnum % 4;
	X = PX = rnum % (BOARD_COL - NUM(R, 16));
}

// set the value of the board for a particular (x,y,r) piece
static void set_piece(i8 x, i8 y, i8 r, u8 v) {
	for (i8 i = 0; i < 8; i += 2)
		BOARD[NUM(r, i * 2) + y][NUM(r, (i * 2) + 2) + x] = v;
}

// move a piece from old (p*) coords to new
static void update_piece() {
	set_piece(PX, PY, PR, 0);
	set_piece(PX = X, PY = Y, PR = R, P + 1);
}

// remove line(s) from the board if they're full
static void remove_line() {
	for (u8 row = Y; row <= Y + NUM(R, 18); row++) {
		for (u8 i = 0; i < 10; i++) {
			if (!BOARD[row][i])
				goto CONTINUE;
		}

		for (i8 i = row - 1; i > 0; i--)
			memcpy(&BOARD[i + 1][0], &BOARD[i][0], sizeof(**BOARD) * BOARD_COL);

		//memset(&board[0][0], 0, 10);
		SCORE++;

		CONTINUE:;
	}
}

// check if placing p at (x,y,r) will be a collision
static u8 check_hit(i8 x, i8 y, i8 r) {
	if (y + NUM(r, 18) > 19) return 1;

	set_piece(PX, PY, PR, 0);

	u8 is_hit = 0;
	for (u8 i = 0; i < 8; i += 2) {
		if (BOARD[y + NUM(r, i * 2)][x + NUM(r, (i * 2) + 2)]) {
			is_hit = 1;
			break;
		}
	}

	set_piece(PX, PY, PR, P + 1);
	return is_hit;
}

static void land_piece() {
	while (!check_hit(X, Y + 1, R)) {
		Y++;
		update_piece();
	}
	
	remove_line();
	new_piece();
}

static void rotate_piece(u8 flag) {
	R = (flag == LEFT)  ? LR(R, 4) : RR(R, 4);
	while (X + NUM(R, 16) > 9)
		X--;
	if (check_hit(X, Y, R)) {
		X = PX;
		R = PR;
	}
}


// drawing mechanism=================================================

#define BOARD_START_ROW 3
#define BOARD_START_COL ((DMAT_COL - BOARD_COL) / 2)

#define BOARD_END_ROW (BOARD_START_ROW + BOARD_ROW)
#define BOARD_END_COL (BOARD_START_COL + BOARD_COL)

static inline void draw_border() {
	for (coord_t c = BOARD_START_COL - 1; c < BOARD_END_COL + 1; c++) {
		DMAT_set_rgb_bit(BOARD_START_ROW - 1, c, CG | CR | CB);
		DMAT_set_rgb_bit(BOARD_END_ROW, c, CG | CR | CB);
	}
	
	for (coord_t r = BOARD_START_ROW - 1; r < BOARD_END_ROW + 1; r++) {
		DMAT_set_rgb_bit(r, BOARD_START_COL - 1, CG | CR | CB);
		DMAT_set_rgb_bit(r, BOARD_END_COL, CG | CR | CB);
	}
}

static inline void draw_board() {
	for (coord_t r = 0; r < BOARD_ROW; r++)
	for (coord_t c = 0; c < BOARD_COL; c++)
	DMAT_set_rgb_bit(BOARD_START_ROW + r, BOARD_START_COL + c, BOARD[r][c]);
}

#define SCORE_ROW (BOARD_END_ROW + 2)
#define SCORE_COL(i) (DMAT_COL - DMAT_DIGIT_RATIO_W - (i) * (DMAT_DIGIT_RATIO_W + 1))

void draw_score(coord_t row) {
	u16 score = SCORE;
	for (coord_t i = 0; i < 4; i++) {
		DMAT_draw_digit_bit(row, SCORE_COL(i), score % 10, CR, 1);
		score /= 10;
	}
}

// draw the board and score
static void frame() {
	DMAT_start_write();
	
	draw_border();
	draw_board();
	draw_score(SCORE_ROW);

	DMAT_end_write(DMAT_CLR);
}

// ==================================================================

void tetris_init(int seed) {
	memset((void*)BOARD, 0, sizeof(BOARD));

	srand(seed);
	new_piece();
}

#define LnR (LEFT | RIGHT)
void tetris_process_input(u8 input) {
	static DEF_PREV_DIGITAL_READ(0);
	static u32 PREV_MS_BTN_LR;
	static u32 WAIT_TIME_BTN_LR;

	u8 changed = 0;
	
	/* LEFT & RIGHT: horizontal move */
	if ((LnR & input) == LnR) input &= ~LnR;

	if ( IS_RISING(0, input, LnR) ) {
		WAIT_TIME_BTN_LR = LR_SEMICONT_TERM;
		PREV_MS_BTN_LR = millis();

		if (input & LEFT)
			X > 0 && !check_hit(X - 1, Y, R) && X--;
		else
			X + NUM(R, 16) < 9 && !check_hit(X + 1, Y, R) && X++;
		
		changed = 1;
	}
	else if ( IS_HIGH(input, LnR) ) {
		u32 curr = millis();
		
		if (TIME_OUTI(curr, PREV_MS_BTN_LR, WAIT_TIME_BTN_LR)) {
			WAIT_TIME_BTN_LR = LR_SEMICONT_CONT;

			if (input & LEFT)
				X > 0 && !check_hit(X - 1, Y, R) && X--;
			else
				X + NUM(R, 16) < 9 && !check_hit(X + 1, Y, R) && X++;
			
			changed = 1;
		}
	}
	
	/* UP: rotate */
	if ( IS_RISING(0, input, UP) )
		rotate_piece(RIGHT), changed = 1;

	/* DOWN: land */
	if ( IS_RISING(0, input, DOWN) ) {
#ifdef LAND_CONTROL
		land_piece(), changed = 1;
#else
		rotate_piece(LEFT), changed = 1;
#endif
	}
	
	if (changed) {
		update_piece();
		frame();
	}

	UPDATE_PREV_DIGITAL_READ(0, input);
}

u8 tetris_do_tick() {
	if (check_hit(X, Y + 1, R)) {
		if (!Y) return 0;
		remove_line();
		new_piece();
	} else {
		Y++;
		update_piece();
	}
	
	frame();
	
	return 1;
}