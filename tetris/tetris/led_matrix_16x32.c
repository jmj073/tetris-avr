/*
 * led_matrix_16x32.c
 *
 * Created: 2022-06-30 20:09:51
 *  Author: JMJ073
 */ 

#include "led_matrix_16x32.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#define GAMMA_CORRECTION_ENABLE

#ifdef GAMMA_CORRECTION_ENABLE
#include <avr/pgmspace.h>
const u8 gamma8[] PROGMEM = { // 1.8
	0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  2,
	2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  6,
	6,  6,  7,  7,  8,  8,  8,  9,  9, 10, 10, 10, 11, 11, 12, 12,
	13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21,
	21, 22, 22, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31,
	32, 32, 33, 34, 35, 35, 36, 37, 38, 38, 39, 40, 41, 41, 42, 43,
	44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 53, 54, 55, 56, 57,
	58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
	74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 86, 87, 88, 89, 90,
	91, 92, 93, 95, 96, 97, 98, 99,100,102,103,104,105,107,108,109,
	110,111,113,114,115,116,118,119,120,122,123,124,126,127,128,129,
	131,132,134,135,136,138,139,140,142,143,145,146,147,149,150,152,
	153,154,156,157,159,160,162,163,165,166,168,169,171,172,174,175,
	177,178,180,181,183,184,186,188,189,191,192,194,195,197,199,200,
	202,204,205,207,208,210,212,213,215,217,218,220,222,224,225,227,
	229,230,232,234,236,237,239,241,243,244,246,248,250,251,253,255,
};
#endif /* GAMMA_CORRECTION_ENABLE */

static u8 __BRIGHTNESS;

static u8 __LEDMAT_BUFFER[2][LEDMAT_SECTION][LEDMAT_COL];
static u8 __LEDMAT_CURR_BUF = 0;

#define LEDMAT_FRONT_BUF ((const u8(*)[LEDMAT_COL])__LEDMAT_BUFFER[__LEDMAT_CURR_BUF])
#define LEDMAT_BACK_BUF (__LEDMAT_BUFFER[__LEDMAT_CURR_BUF ^ 1])

static inline void _addr(u8 addr) {
	PORT(LEDMAT_CR) = (PORT(LEDMAT_CR) & ~0x07) | (addr & 0x07);
}

static inline void _rgb(u8 rgb) {
	PORT(LEDMAT_RGB) = (PORT(LEDMAT_RGB) & ~LEDMAT_RGB_ALL) | rgb;
}

static inline void _rgb1(u8 rgb1) {
	PORT(LEDMAT_RGB) = (PORT(LEDMAT_RGB) & ~LEDMAT_RGB1) | rgb1;
}

static inline void _rgb2(u8 rgb2) {
	PORT(LEDMAT_RGB) = (PORT(LEDMAT_RGB) & ~LEDMAT_RGB2) | (rgb2 << 3);
}

static inline void _clock() {
	PORT(LEDMAT_CR) |= _BV(LEDMAT_CLK);
	PORT(LEDMAT_CR) &= ~_BV(LEDMAT_CLK);
}

static inline void _latch() {
	PORT(LEDMAT_CR) |= _BV(LEDMAT_LAT);
	PORT(LEDMAT_CR) &= ~_BV(LEDMAT_LAT);
}

static inline void _out_enable() {
	PORT(LEDMAT_CR) &= ~_BV(LEDMAT_OE);
}

static inline void _out_disable() {
	PORT(LEDMAT_CR) |= _BV(LEDMAT_OE);
}

static inline u8 _correction(u8 brightness) {
#ifdef GAMMA_CORRECTION_ENABLE
	return pgm_read_byte(&gamma8[brightness]);
#else /* GAMMA_CORRECTION_ENABLE */
	return brightness;
#endif /* GAMMA_CORRECTION_ENABLE */
}

static inline void _tx_section(const u8* RGBs) {
	for (u8 i = 0; i < LEDMAT_COL; i++) {
		_rgb(*RGBs++);
		_clock();
	}
}

static inline void _refresh() {
	static u8 section;

	_tx_section(LEDMAT_FRONT_BUF[section]);
	
	_out_disable();
	_latch();
	_addr(section);
	_out_enable();
	
	section = RR(section, LEDMAT_SECTION);
}

/* refresher for LED matrix & control brightness */
static inline void _timer2_init()
{
	TCCR2 |= (
		_BV(WGM21) | _BV(WGM20)	| // Fast PWM
		_BV(CS21) | _BV(CS20)	| // clk/64
	0);
	OCR2 = _correction(__BRIGHTNESS);
	TIMSK |= _BV(TOIE2) | _BV(OCIE2);
}

static inline void _init_port()
{
	DDR(LEDMAT_RGB) |= LEDMAT_RGB_ALL;
	DDR(LEDMAT_CR) |= LEDMAT_CR_ALL;
}

void LEDMAT_init()
{
	__BRIGHTNESS = LEDMAT_DEFAULT_BRIGHTNESS;
	_init_port();
	_out_disable();
	_timer2_init();
}

void LEDMAT_swap_buffer()
{
	__LEDMAT_CURR_BUF ^= 1;
}

void LEDMAT_copy_buffer()
{
	memcpy(LEDMAT_BACK_BUF, LEDMAT_FRONT_BUF, sizeof(LEDMAT_BACK_BUF));
}

void LEDMAT_set_rgb_bit(u8 r, u8 c, u8 rgb)
{
	if (r < 8) {
		LEDMAT_BACK_BUF[r][c]
		= (LEDMAT_BACK_BUF[r][c] & ~LEDMAT_RGB1)
		| (rgb & LEDMAT_RGB1);
	} else {
		LEDMAT_BACK_BUF[r & 7][c]
		= (LEDMAT_BACK_BUF[r & 7][c] & ~LEDMAT_RGB2)
		| ((rgb << 3) & LEDMAT_RGB2);
	}
}

u8 LEDMAT_get_rgb_bit(u8 r, u8 c)
{
	return r < 8 ?
	LEDMAT_BACK_BUF[r][c] & LEDMAT_RGB1:
	LEDMAT_BACK_BUF[r & 7][c] >> 3;
}

void LEDMAT_fill_rgb_bit(u8 rgb)
{
	rgb &= LEDMAT_RGB1;
	rgb = (rgb << 3) | rgb;
	
	memset(LEDMAT_BACK_BUF, rgb, sizeof(LEDMAT_BACK_BUF));
}

void LEDMAT_set_brightness(u8 brightness)
{
	if (brightness >= LEDMAT_MAX_BRIGHTNESS) {		
		u8 timsk = TIMSK;
		TIFR |= ~timsk & _BV(TOV2);
		TIMSK = (timsk & ~_BV(OCIE2)) | _BV(TOIE2);
		_out_enable();
		__BRIGHTNESS = LEDMAT_MAX_BRIGHTNESS;
	} else if (brightness <= LEDMAT_MIN_BRIGHTNESS) {
		TIMSK &= ~(_BV(TOIE2) | _BV(OCIE2));
		_out_disable();
		__BRIGHTNESS = LEDMAT_MIN_BRIGHTNESS;
	} else {
		OCR2 = _correction(brightness);
		u8 timsk = TIMSK;
		TIFR |= ~timsk & (_BV(TOV2) | _BV(OCF2));
		TIMSK = timsk | _BV(TOIE2) | _BV(OCIE2);
		__BRIGHTNESS = brightness;
	}
}

u8 LEDMAT_get_brightness()
{
	return __BRIGHTNESS;
}

/* LED matrix refresh */
ISR(TIMER2_OVF_vect)
{
	_refresh();
}

/* brightness control */
ISR(TIMER2_COMP_vect)
{
	_out_disable();
}