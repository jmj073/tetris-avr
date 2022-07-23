/*
 * LEDMatrix16x32.c
 *
 * Created: 2022-06-29 15:34:49
 * Author : JMJ073
 * 
 * Tetris original source code: https://github.com/taylorconor/tinytetris
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
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

/* refresher for LED matrix & control brightness */
static inline void TC2_init() {
	TIMSK |= _BV(TOIE2);
	TCCR2 |= TC_REFRESH_CLOCK_SELECT;
	OCR2 = DEFAULT_LEDMAT_BRIGHTNESS;
	TIMSK |= _BV(OCIE2);
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

// =================================================================

static void draw_screen_from_eeprom(u8* addr) {
	for (coord_t r = 0; r < DMAT_ROW; r++)
	for (coord_t c = 0; c < DMAT_COL / 2; c++) {
		u8 rgb = eeprom_read_byte(addr++);
		DMAT_set_rgb_bit(r, c, rgb & 7);
		DMAT_set_rgb_bit(r, c + DMAT_COL / 2, rgb >> 3);
	}
}

#define STANDBY_SCREEN_ADDR		((u8*)0)
#define GAMEOVER_SCREEN_ADDR	((u8*)(DMAT_ROW * (DMAT_COL / 2)))

static void standby_screen() {
	DMAT_start_write();

	draw_screen_from_eeprom(STANDBY_SCREEN_ADDR);

	DMAT_end_write(DMAT_CLR);
}

static void gameover_screen() {
	DMAT_start_write();

	draw_screen_from_eeprom(GAMEOVER_SCREEN_ADDR);
	draw_score(DMAT_ROW / 2 + 3);

	DMAT_end_write(DMAT_CLR);
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

static void standby_screen_process_input() {
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
		if (input & _BV(BtN_RIGHT)) {
			break;
		}
		
		_delay_ms(10);
	}
}

static void run()
{
	DEF_PREV_MS(INPUT_POLL_MS);
	DEF_PREV_MS(TICK_MS);
	
	tetris_init(ADC_read());
	standby_screen();
	
	_delay_ms(500);
	standby_screen_process_input();
	countdown(3);
	
	u32 init_value = millis();
	PREV_MS(INPUT_POLL_MS) = init_value;
	PREV_MS(TICK_MS) = init_value;
	
	loop {
		u32 curr = millis();

		if (TIME_OUT_MSI(curr, INPUT_POLL_MS))
			tetris_process_input(BtN_PRESSED());
		if (TIME_OUT_MSI(curr, TICK_MS))
			if (!tetris_do_tick()) break;
	}
	
	gameover_screen();
	
	_delay_ms(1000);
	while (!BtN_PRESSED());
}

int main()
{
	init();
	standby_screen();
	
	sei();
	
	loop {
		run();
	}
}

#if 0

int main() {
	DEF_PREV_MS(INPUT_POLL_MS);
	DEF_PREV_MS(TICK_MS);
	
	init();
	tetris_init(ADC_read());
	standby_screen();
	sei();
	
	_delay_ms(500);
	standby_screen_process_input();
	countdown(3);

	u32 init_value = millis();
	PREV_MS(INPUT_POLL_MS) = init_value;
	PREV_MS(TICK_MS) = init_value;
	
	loop {
		u32 curr = millis();

		if (TIME_OUT_MSI(curr, INPUT_POLL_MS))
			tetris_process_input(BtN_PRESSED());
		if (TIME_OUT_MSI(curr, TICK_MS))
			if (!tetris_do_tick()) break;
	}
	
	gameover_screen();
	
	_delay_ms(1000);
	while (!BtN_PRESSED());
	SW_RESET();
}

#endif

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