/*
 * teris.c
 *
 * Created: 2022-07-05 22:09:27
 *  Author: JMJ
 */ 

#include <avr/wdt.h>
#include <stdlib.h>

#include "pins.h"
#include "settings.h"
#include "display_matrix_api.h"
#include "timer.h"

#define UP		BtN_UP
#define DOWN	BtN_DOWN
#define LEFT	BtN_LEFT
#define RIGHT	BtN_RIGHT

#define SW_RESET()		    \
do                          \
{                           \
	wdt_enable(WDTO_15MS);  \
	for(;;);                \
} while(0)

// block layout is: {w-1,h-1}{x0,y0}{x1,y1}{x2,y2}{x3,y3} (two bits each)

#define BOARD_ROW	20
#define BOARD_COL	10

static u8 X, Y, R, PX, PY, PR, P;
static u16 SCORE = 0;
static u8 BOARD[BOARD_ROW][BOARD_COL];

static const i32 block[7][4] = {
	{ 431424, 598356, 431424, 598356 },
	{ 427089, 615696, 427089, 615696 },
	{ 348480, 348480, 348480, 348480 },
	{ 599636, 431376, 598336, 432192 },
	{ 411985, 610832, 415808, 595540 },
	{ 247872, 799248, 247872, 799248 },
	{ 614928, 399424, 615744, 428369 },
};

// extract a 2-bit number from a block entry
static inline i8 NUM(i8 x, i8 y) { return (block[P][x] >> y) & 3; }

// create a new piece, don't remove old one (it has landed and should stick)
static void new_piece() {
	int rnum = rand();
	Y = PY = 0;
	P = rnum % 7;
	R = PR = rnum % 4;
	X = PX = rnum % (10 - NUM(R, 16));
}

// draw the board and score
static void frame() {
	DMAT_start_write();
	
	for (i8 r = 0; r < BOARD_ROW; r++)
		for (i8 c = 0; c < BOARD_COL; c++)
			DMAT_set_rgb_bit(r, c, BOARD[r][c]);

	DMAT_end_write(0);
}

// set the value of the board for a particular (x,y,r) piece
static void set_piece(i8 x, i8 y, i8 r, i8 v) {
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

static void rotate_piece() {
	R = RR(R, 4);
	while (X + NUM(R, 16) > 9)
	X--;
	if (check_hit(X, Y, R)) {
		X = PX;
		R = PR;
	}
}


#define LnR (_BV(LEFT) | _BV(RIGHT))
void tetris_process_input(u8 input) {
	static DEF_PREV_DIGITAL_READ(0);
	static uint32_t PREV_MS_BTN_LR;
	static uint32_t WAIT_TIME_BTN_LR;

	u8 changed = 0;
	
	/* LEFT & RIGHT: horizontal move */
	if ((LnR & input) == LnR) input &= ~LnR;

	if ( IS_RISING(0, input, LnR) ) {
		WAIT_TIME_BTN_LR = LR_SEMICONT_TERM;
		PREV_MS_BTN_LR = millis();

		if (input & _BV(LEFT))
			X > 0 && !check_hit(X - 1, Y, R) && X--;
		else
			X + NUM(R, 16) < 9 && !check_hit(X + 1, Y, R) && X++;
		
		changed = 1;
	}
	else if ( IS_HIGH(input, LnR) ) {
		uint32_t curr = millis();
		
		if (TIME_OUTI(curr, PREV_MS_BTN_LR, WAIT_TIME_BTN_LR)) {
			WAIT_TIME_BTN_LR = LR_SEMICONT_CONT;

			if (input & _BV(LEFT))
				X > 0 && !check_hit(X - 1, Y, R) && X--;
			else
				X + NUM(R, 16) < 9 && !check_hit(X + 1, Y, R) && X++;
			
			changed = 1;
		}
	}
	
	/* UP: rotate */
	if ( IS_RISING(0, input, _BV(UP)) )
		rotate_piece(), changed = 1;

	/* DOWN: land */
	if ( IS_RISING(0, input, _BV(DOWN)) )
		land_piece(), changed = 1;
	
	if (changed) {
		update_piece();
		frame();
	}

	UPDATE_PREV_DIGITAL_READ(0, input);
}

void tetris_do_tick() {
	if (check_hit(X, Y + 1, R)) {
		if (!Y) SW_RESET();
		remove_line();
		new_piece();
	} else {
		Y++;
		update_piece();
	}
	
	frame();
}

void tetris_init() {
	new_piece();
}