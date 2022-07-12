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
#include <avr/wdt.h>
#include <util/delay.h>

#include "settings.h"
#include "display_matrix_api.h"
#include "timer.h"
#include "pins.h"
#include "tetris.h"

#define SW_RESET()		    \
do                          \
{                           \
	wdt_enable(WDTO_15MS);  \
	for(;;);                \
} while(0)

// initilize========================================================

/* refresher for LED matrix */
static inline void TC2_init() {
	TIMSK |= _BV(TOIE2);
	TCCR2 |= TC_REFRESH_CLOCK_SELECT;
}

static inline void BtN_init() {
	DDR(BtN_PIN) &= ~BtN_ALL_PINS; // input
	PORT(BtN_PIN) |= BtN_ALL_PINS; // built-in pull-up
}

static void ADC_init(uint8_t channel)
{
	ADMUX |= _BV(REFS0); // AVCC를 기준 전압으로 선택
	
	ADCSRA |= 0x07; // 분주비 설정
	ADCSRA |= _BV(ADEN); // ADC 활성화
	
	ADMUX = ((ADMUX & 0xE0) | channel); // 채널 선택
}

static void init() {
	BtN_init();
	LEDMAT_init();
	TC2_init();
	timer0_init();
	ADC_init(0);
}

// =================================================================

int ADC_read()
{
	ADCSRA |= _BV(ADSC); // 변환 시작
	while ( !(ADCSRA & _BV(ADIF)) ); // 변환 대기
	return ADC;
}

#define COUNT_ROW ((DMAT_ROW - DMAT_DIGIT_RATIO_H * 2) / 2)
#define COUNT_COL ((DMAT_COL - DMAT_DIGIT_RATIO_W * 2) / 2)
static void countdown(u8 cnt) {
	for (u8 i = cnt; i > 0; i--) {
		DMAT_start_write();
		DMAT_draw_digit_bit(COUNT_ROW, COUNT_COL, i, CR | CG | CB, 2);
		DMAT_end_write(DMAT_CLR);

		_delay_ms(1000);
	}
}

int main() {
	init();
	
	tetris_init(ADC_read());

	sei();
	
	_delay_ms(500);
	while (!BtN_PRESSED());
	countdown(3);

	u32 init_value = millis();
	DEF_PREV_MS(INPUT_POLL_MS) = init_value;
	DEF_PREV_MS(TICK_MS) = init_value;

	loop {
		u32 curr = millis();

		if (TIME_OUT_MSI(curr, INPUT_POLL_MS))
			tetris_process_input(BtN_PRESSED());
		if (TIME_OUT_MSI(curr, TICK_MS))
			if (!tetris_do_tick()) break;
	}
	
	_delay_ms(1000);
	while (!BtN_PRESSED());
	SW_RESET();
}

// TESTS============================================================

#if 0
// rect draw test

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


#if 0
// standby screen and eeprom test

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