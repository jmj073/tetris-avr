/*
 * teris.c
 *
 * Created: 2022-07-05 22:09:27
 * Author: JMJ
 * Tetris original source code: https://github.com/taylorconor/tinytetris
 */ 

#include <stdlib.h>

#include "tetris.h"
#include "pins.h"
#include "settings.h"
#include "timer.h"

#define UP		_BV(BTN_UP)
#define DOWN	_BV(BTN_DOWN)
#define LEFT	_BV(BTN_LEFT)
#define RIGHT	_BV(BTN_RIGHT)

// block layout is: {h-1,w-1}{x0,y0}{x1,y1}{x2,y2}{x3,y3} (two bits each, total 20 bits)

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
static inline u8 NUM(u8 r, u8 n) { return (BLOCK[P][r] >> n) & 3; }
static inline u8 _width(u8 r) { return NUM(r, 16) + 1; }
static inline u8 _height(u8 r) { return NUM(r, 18) + 1; }
static inline u8 _getx(u8 r, u8 i) { return NUM(r, i * 4 + 2); }
static inline u8 _gety(u8 r, u8 i) { return NUM(r, i * 4); }

// create a new piece, don't remove old one (it has landed and should stick)
static void new_piece() {
	int rnum = rand();
	Y = PY = 0;
	P = rnum % 7;
	R = PR = rnum % 4;
	X = PX = rnum % (BOARD_COL - _width(R));
}

// set the value of the board for a particular (x,y,r) piece
static void set_piece(u8 x, u8 y, u8 r, u8 v) {
	for (u8 i = 0; i < 4; i++) {
		BOARD[y + _gety(r, i)][x + _getx(r, i)] = v;
	}
}

// move a piece from old (p*) coords to new
static void update_piece() {
	set_piece(PX, PY, PR, 0);
	set_piece(PX = X, PY = Y, PR = R, P + 1);
}

// remove line(s) from the board if they're full
static void remove_line() {
	for (u8 row = Y; row < Y + _height(R); row++) {
		for (u8 i = 0; i < BOARD_COL; i++) {
			if (!BOARD[row][i]) goto CONTINUE;
		}

		for (u8 i = row; i > 0; i--) {
			memcpy(BOARD[i], BOARD[i - 1], sizeof(**BOARD) * BOARD_COL);
		}
		
		SCORE++;
		
		CONTINUE:;
	}
}

// check if placing p at (x,y,r) will be a collision
static u8 check_hit(u8 x, u8 y, u8 r) {
	if (y + _height(r) > BOARD_ROW) return 1;

	set_piece(PX, PY, PR, 0);

	u8 is_hit = 0;
	for (u8 i = 0; i < 4; i++) {
		if (BOARD[y + _gety(r, i)][x + _getx(r, i)]) {
			is_hit = 1;
			break;
		}
	}

	set_piece(PX, PY, PR, P + 1);
	return is_hit;
}

//static void drop_piece() {
	//while (!check_hit(X, Y + 1, R)) {
		//Y++;
		//update_piece();
	//}
	//
	//remove_line();
	//new_piece();
//}

static void rotate_piece(u8 flag) {
	R = (flag == LEFT)  ? LR(R, 4) : RR(R, 4);
	X = min(X, BOARD_COL - _width(R));
	if (check_hit(X, Y, R)) {
		X = PX;
		R = PR;
	}
}

static void move_piece(u8 flag)
{
	if (flag & LEFT) {
		(void)(X > 0 && !check_hit(X - 1, Y, R) && X--);
	} else {
		(void)(X + _width(R) < BOARD_COL && !check_hit(X + 1, Y, R) && X++);
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
	for (coord_t r = 0; r < BOARD_ROW; r++) {
		for (coord_t c = 0; c < BOARD_COL; c++) {
			DMAT_set_rgb_bit(BOARD_START_ROW + r, BOARD_START_COL + c, BOARD[r][c]);
		}
	}
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

/* draw the board and score */
static void frame()
{
	DMAT_clear();
	
	draw_border();
	draw_board();
	draw_score(SCORE_ROW);

	DMAT_update(0);
}

// ==================================================================

#define LnR (LEFT | RIGHT)
void tetris_process_input(u8 curr_input, u8 prev_input)
{
	static u32 PREV_MS_BTN_LR;
	static u32 WAIT_TIME_BTN_LR;

	u8 changed = 0;

	/* LEFT & RIGHT: horizontal move */
	if ( ~prev_input & curr_input & LnR ) {
		WAIT_TIME_BTN_LR = LR_SEMICONT_TERM;
		PREV_MS_BTN_LR = millis();

		move_piece(curr_input);
		
		changed = 1;
	} else if ( curr_input & LnR ) {
		u32 curr = millis();
		
		if (TIME_OUTI(curr, &PREV_MS_BTN_LR, WAIT_TIME_BTN_LR)) {
			WAIT_TIME_BTN_LR = LR_SEMICONT_CONT;
			move_piece(curr_input);
			changed = 1;
		}
	}
	
	/* UP: rotate to the right */
	if ( ~prev_input & curr_input & UP ) {
		rotate_piece(RIGHT), changed = 1;
	}

	/* DOWN: rotate to the left */
	if ( ~prev_input & curr_input & DOWN ) {
		rotate_piece(LEFT), changed = 1;
	}
	
	if (changed) {
		update_piece();
		frame();
	}
}

u8 tetris_do_tick()
{
	if (check_hit(X, Y + 1, R)) {
		if (!Y) return 0;
		remove_line();
		new_piece();
		if (check_hit(X, Y, R)) return 0;
	} else {
		Y++;
	}

	update_piece();
	frame();
	
	return 1;
}

void tetris_init()
{	
	memset((void*)BOARD, 0, sizeof(BOARD));
	new_piece();
	update_piece();
	SCORE = 0;
}