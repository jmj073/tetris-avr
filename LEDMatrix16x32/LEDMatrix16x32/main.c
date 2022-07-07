/*
 * LEDMatrix16x32.c
 *
 * Created: 2022-06-29 15:34:49
 * Author : JMJ073
 * 
 * Tetris original source code: https://github.com/taylorconor/tinytetris
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "settings.h"
#include "display_matrix_api.h"
#include "timer.h"
#include "pins.h"
#include "tetris.h"


/* refresher */
static inline void TC2_init() {
	TIMSK |= _BV(TOIE2);
	TCCR2 |= TC_REFRESH_CLOCK_SELECT;
}

static inline void BtN_init() {
	DDR(BtN_PIN) |= BtN_ALL_PINS; // input
	PORT(BtN_PIN) |= BtN_ALL_PINS; // built-in pull-up
}

static void init() {
	BtN_init();
	DMAT_init();
	TC2_init();
	timer0_init();
	tetris_init();
}

void standby_screen() {
	u8* addr = (u8*)0;
	
	DMAT_start_write();
	
	for (u16 r = 0; r < 32; r++)
	for (u16 c = 0; c < 16 / 2; c++) {
		u8 rgb = eeprom_read_byte(addr++);
		DMAT_set_rgb_bit(r, c, rgb & 7);
		DMAT_set_rgb_bit(r, c + 16 / 2, rgb >> 3);
	}
	
	DMAT_end_write(DMAT_NCOPY);
}

int main() {
	init();

	sei();

	standby_screen();
	
	while (!BtN_PRESSED());

	u32 curr = millis();
	DEF_PREV_MS(INPUT_POLL_MS) = curr;
	DEF_PREV_MS(TICK_MS) = curr;

	loop {
		u32 curr = millis();

		if (TIME_OUT_MSI(curr, INPUT_POLL_MS)) {
			tetris_process_input(BtN_PRESSED());
		}
		if (TIME_OUT_MSI(curr, TICK_MS)) {
			tetris_do_tick();
		}
	}
}



//int main() {
	//DMAT_init();
	//TC2_init();
//
	//sei();
	//
	//DMAT_start_write();
	//
	//for (u8 i = 0; i < 16; i++) {
		//DMAT_set_rgb_bit(i, i, CR | CG | CB);
		//DMAT_set_rgb_bit(i, 15 - i, CR | CG | CB);
	//}
			//
	//DMAT_end_write(0);
	//
	//loop;
//}


#if 0
// standby screen and eeprom test

#include <avr/eeprom.h>

int main() {
	DMAT_init();
	TC2_init();
	
	u8* addr = (u8*)0;
	
	DMAT_start_write();
	
	for (u16 r = 0; r < 32; r++)
		for (u16 c = 0; c < 16 / 2; c++) {
			u8 rgb = eeprom_read_byte(addr++);
			DMAT_set_rgb_bit(r, c, rgb & 7);
			DMAT_set_rgb_bit(r, c + 16 / 2, rgb >> 3);
		}
		
	DMAT_end_write(DMAT_NCOPY);
	
	sei();
	
	loop;
}

#endif // standby screen and eeprom test