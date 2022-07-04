/*
 * LEDMatrix16x32.c
 *
 * Created: 2022-06-29 15:34:49
 * Author : JMJ073
 * 
 * Tetris origital source code: https://github.com/taylorconor/tinytetris
 */ 

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"
#define LED16X32_SHORT_MACRO
#include "led_matrix_16x32.h"
#include "timer.h"
#include "pins.h"

#define SW_RESET()		    \
do                          \
{                           \
	wdt_enable(WDTO_15MS);  \
	for(;;);                \
} while(0) 

// block layout is: {w-1,h-1}{x0,y0}{x1,y1}{x2,y2}{x3,y3} (two bits each)

static i8 x, y;
static i32 r, px, py, pr, p, board[20][10], score = 0;

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
static inline i8 NUM(i8 x, i8 y) { return (block[p][x] >> y) & 3; }

// create a new piece, don't remove old one (it has landed and should stick)
static void new_piece() {
	int rnum = rand();
	y = py = 0;
	p = rnum % 7;
	r = pr = rnum % 4;
	x = px = rnum % (10 - NUM(r, 16));
}

// draw the board and score
static void frame() {
	for (i8 i = 0; i < 20; i++)
		for (i8 j = 0; j < 10; j++)
			LEDMAT_set_rgb(j, i, board[i][j]);
}

// set the value of the board for a particular (x,y,r) piece
static void set_piece(i8 x, i8 y, i8 r, i8 v) {
	for (i8 i = 0; i < 8; i += 2)
	board[NUM(r, i * 2) + y][NUM(r, (i * 2) + 2) + x] = v;
}

// move a piece from old (p*) coords to new
static void update_piece() {
	set_piece(px, py, pr, 0);
	set_piece(px = x, py = y, pr = r, p + 1);
}

// remove line(s) from the board if they're full
static void remove_line() {
	for (i8 row = y; row <= y + NUM(r, 18); row++) {
		for (i8 i = 0; i < 10; i++) {
			if (!board[row][i])
				goto CONTINUE;
		}


		for (i8 i = row - 1; i > 0; i--)
			memcpy(&board[i + 1][0], &board[i][0], 40);

		memset(&board[0][0], 0, 10);
		score++;

CONTINUE:;
	}
}

// check if placing p at (x,y,r) will be a collision
static i32 check_hit(i8 x, i8 y, i8 r) {
	if (y + NUM(r, 18) > 19) return 1;

	set_piece(px, py, pr, 0);

	u8 is_hit = 0;
	for (i32 i = 0; i < 8; i += 2) {
		if (board[y + NUM(r, i * 2)][x + NUM(r, (i * 2) + 2)]) {
			is_hit = 1;
			break;
		}
	}

	set_piece(px, py, pr, p + 1);
	return is_hit;
}

static void land_piece() {
	while (!check_hit(x, y + 1, r)) {
		y++;
		update_piece();
	}
	remove_line();
	new_piece();
}

static void rotate_piece() {
	r = RR(r, 4);
	while (x + NUM(r, 16) > 9)
		x--;
	if (check_hit(x, y, r)) {
		x = px;
		r = pr;
	}
}

#define BtN_LR (_BV(BtN_LEFT) | _BV(BtN_RIGHT))
void tetris_process_input(u8 input) {
	static DEF_PREV_DIGITAL_READ();
	static uint32_t btn_lr_prev_ms;
	static uint32_t btn_lr_wait;

	u8 changed = 0;
	
	/* UP: rotate */
	if ( IS_RISING(input, _BV(BtN_UP)) )
		rotate_piece(), changed = 1;

	/* DOWN: land */
	if ( IS_RISING(input, _BV(BtN_DOWN)) )
		land_piece(), changed = 1;
	

	/* LEFT & RIGHT: horizontal move */
	if ((BtN_LR & input) == BtN_LR)
		input &= ~BtN_LR;

	if ( IS_RISING(input, BtN_LR) ) {
		btn_lr_wait = LR_SEMICONT_TERM;
		btn_lr_prev_ms = millis();

		if (input & _BV(BtN_LEFT))
			x > 0 && !check_hit(x - 1, y, r) && x--;
		else 
			x + NUM(r, 16) < 9 && !check_hit(x + 1, y, r) && x++;
			
		changed = 1;
	}
	else if ( IS_HIGH(input, BtN_LR) ) {
		uint32_t curr = millis();
		if (curr - btn_lr_prev_ms >= btn_lr_wait) {
			btn_lr_prev_ms += btn_lr_wait;
			btn_lr_wait = LR_SEMICONT_CONT;

			if (input & _BV(BtN_LEFT))
				x > 0 && !check_hit(x - 1, y, r) && x--;
			else
				x + NUM(r, 16) < 9 && !check_hit(x + 1, y, r) && x++;
				
			changed = 1;
		}
	}
	
	if (changed) {
		update_piece();
		frame();
	}

	UPDATE_PREV_DIGITAL_READ(input);
}

static void do_tick() {
	if (check_hit(x, y + 1, r)) {
		if (!y) SW_RESET(); 
		remove_line();
		new_piece();
	} else {
		y++;
		update_piece();
	}
	frame();
}

/* refresher */
static void TC2_init() {
	TIMSK |= _BV(TOIE2);
	TCCR2 |= TC_REFRESH_CLOCK_SELECT;
}

static void BtN_init() {
	PORT(BtN_PIN) |= BtN_ALL_PINS; // built-in pull-up
	DDR(BtN_PIN) |= BtN_ALL_PINS; // input
}

static void init() {
	BtN_init();
	LMAT_init();
	TC2_init();
	timer0_init();
}

int main() {
	DEF_PREV_MS(INPUT_POLL_MS) = 0;
	DEF_PREV_MS(TICK_MS) = 0;

	init();	
	new_piece();
	sei();

	loop {
		u32 curr = millis();

		if (TIME_OUT_MSI(curr, INPUT_POLL_MS)) {
			tetris_process_input(BtN_PRESSED());
		}
		if (TIME_OUT_MSI(curr, TICK_MS)) {
			do_tick();
		}
	}
}


