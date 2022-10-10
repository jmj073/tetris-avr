/*
 * LEDMatrix16x32.c
 *
 * Created: 2022-06-29 15:34:49
 * Author : JMJ073
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "settings.h"
#include "display_matrix_api.h"
#include "timer.h"
#include "pins.h"
#include "tetris.h"
#include "images.h"

// initilize========================================================

/* refresher for LED matrix & control brightness */
static inline void timer2_init()
{
	TIMSK |= _BV(TOIE2);
	TCCR2 |= TC_REFRESH_CLOCK_SELECT;
	OCR2 = DEFAULT_LEDMAT_BRIGHTNESS;
	TIMSK |= _BV(OCIE2);
}

static inline void BtN_init()
{
	DDR(BtN_PIN) &= ~BtN_ALL_PINS; // input
	PORT(BtN_PIN) |= BtN_ALL_PINS; // built-in pull-up
}

static void init()
{
	BtN_init();
	LEDMAT_init();
	timer2_init();
	TimeBase_init();
}

// =================================================================

static void set_display_brightness(u8 brightness)
{
	if (brightness >= MAX_LEDMAT_BRIGHTNESS)
		OCR2 = MAX_LEDMAT_BRIGHTNESS;
	else if (brightness <= MIN_LEDMAT_BRIGHTNESS)
		OCR2 = MIN_LEDMAT_BRIGHTNESS;
	else
		OCR2 = brightness;
}

static inline u8 get_display_brightness()
{
	return OCR2;
}

// ~next level~
#define NEXT_LEVEL(level) (((level) + 1) % (MAX_LEVEL + 1))

_Static_assert(17 * MAX_LEVEL < 300, "nope");
static inline u32 level_to_tick(u8 level)
{
	return 300 - 17 * level;
}

// draw=============================================================

// eeprom address
#define STANDBY_SCREEN_ADDR		((u8*)0)
#define GAMEOVER_SCREEN_ADDR	((u8*)(DMAT_ROW * (DMAT_COL / 2)))

static inline void standby_screen()
{
	DMAT_draw_screen_from_pgm((const u8*)STANDBY_IMAGE);
}

static void gameover_screen()
{

	DMAT_draw_screen_from_pgm((const u8*)GAMEOVER_IMAGE);
	draw_score(DMAT_ROW / 2 + 3);

	DMAT_update(0);
}

#define LEVEL_ROW	0
#define LEVEL_COL	0

static void draw_level_bar(u8 level)
{
	coord_t	col = 0;

	for (; col < level; col++)
		DMAT_set_rgb_bit(LEVEL_ROW, LEVEL_COL + col, LEVEL_BAR_COLOR);
	for (; col < MAX_LEVEL; col++)
		DMAT_set_rgb_bit(LEVEL_ROW, LEVEL_COL + col, 0);
}

#define COUNT_ROW ((DMAT_ROW - DMAT_DIGIT_RATIO_H * 2) / 2)
#define COUNT_COL ((DMAT_COL - DMAT_DIGIT_RATIO_W * 2) / 2)
static void countdown(u8 cnt)
{
	for (u8 i = cnt; i > 0; i--) {
		DMAT_clear();
		DMAT_draw_digit_bit(COUNT_ROW, COUNT_COL, i, CR | CG | CB, 2);
		DMAT_update(0);

		_delay_ms(1000);
	}
}

// =================================================================

static u32 menu()
{
	DEF_PREV_MS(LEVEL_CHANGE_MS) = 0;
	static u8 curr_level = DEFAULT_LEVEL;
	
	standby_screen();
	draw_level_bar(curr_level);
	DMAT_update(DMAT_CP);

	loop {
		u8 input = BtN_PRESSED();
		
		if (input & _BV(BtN_UP)) {
			u8 brightness = get_display_brightness();
			if (brightness < MAX_LEDMAT_BRIGHTNESS)
			set_display_brightness(brightness + 1);
		}
		if (input & _BV(BtN_DOWN)) {
			u8 brightness = get_display_brightness();
			if (brightness > MIN_LEDMAT_BRIGHTNESS)
			set_display_brightness(brightness - 1);
		}
		if (input & _BV(BtN_LEFT)) {
			u32 curr_ms = millis();
			if (TIME_OUT_MSA(curr_ms, LEVEL_CHANGE_MS)) {
				draw_level_bar(curr_level = NEXT_LEVEL(curr_level));
				DMAT_update(0);
			}
		}
		if (input & _BV(BtN_RIGHT)) {
			break;
		}
		
		_delay_ms(10);
	}
	
	return level_to_tick(curr_level);
}

static void gameover()
{
	gameover_screen();
	
	_delay_ms(500);
	while (!BtN_PRESSED());
}

static void run()
{
	DEF_PREV_MS(INPUT_POLL_MS);
	u32 TICK_PREV, TICK;

	tetris_init(millis());

	_delay_ms(500);
	TICK = menu();

	countdown(3);

	u32 init_value = millis();
	PREV_MS(INPUT_POLL_MS) = init_value - INPUT_POLL_MS;
	TICK_PREV = init_value - TICK;

	loop {
		u32 curr = millis();

		if (TIME_OUT_MSI(curr, INPUT_POLL_MS))
			tetris_process_input(BtN_PRESSED());

		if (TIME_OUTI(curr, TICK_PREV, TICK))
			if (!tetris_do_tick()) break;
	}

	gameover();

}

int main()
{
	init();
	sei();
	loop run();
}




















































// TESTS==================================================================================

#if 0 /* timer macro utility test */

#define TEST 300

int main()
{
	init();
	sei();
	
	DMAT_set_rgb_bit(0, 0, CR | CG | CB);
	DMAT_update(0);
	
	DEF_PREV_MS(TEST);
	
	loop {
		u32 curr = millis();
		
		if (TIME_OUT_MSI(curr, TEST)) {
			DMAT_update(0);
		}
	}
}

#endif /* timer macro utility test */

#if 0 /* rect draw test */

int main() {
	LEDMAT_init();
	TC2_init();

	DMAT_start_write();
	DMAT_draw_square_fill(10, 3, 5, CR | CG | CB);
	DMAT_end_write(0);
	

	sei();

	loop;
}

#endif /* rect draw test */


#if 0 /* standby screen and eeprom test */

#include <avr/eeprom.h>

int main() {
	LEDMAT_init();
	TC2_init();
	
	u8* addr = (u8*)0;
	
	DMAT_start_write();
	
	for (u16 r = 0; r < 32; r++)
		for (u16 c = 0; c < 16 / 2; c++) {
			u8 rgb = eeprom_read_byte(addr++);
			DMAT_set_rgb_bit(r, c, rgb & 7);
			DMAT_set_rgb_bit(r, c + 16 / 2, rgb >> 3);
		}
		
	DMAT_end_write(DMAT_NCP);
	
	sei();
	
	loop;
}

#endif /* standby screen and eeprom test */