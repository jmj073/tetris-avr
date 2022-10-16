/*
 * led_matrix_16x32.c
 *
 * Created: 2022-06-30 20:09:51
 *  Author: JMJ073
 */ 

#include "led_matrix_16x32.h"
#include <util/delay.h>

static u8 __LEDMAT_BUFFER[2][LEDMAT_SECTION][LEDMAT_COL];
static u8 __LEDMAT_CURR_BUF = 0;

#define LEDMAT_FRONT_BUF (__LEDMAT_BUFFER[__LEDMAT_CURR_BUF])
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
static inline void _tx_section(const u8* RGBs) {
	for (u8 i = 0; i < LEDMAT_COL; i++) {
		_rgb(*RGBs++);
		_clock();
	}
}
static inline void _init_port()
{
	DDR(LEDMAT_RGB) |= LEDMAT_RGB_ALL;
	DDR(LEDMAT_CR) |= LEDMAT_CR_ALL;
}

void LEDMAT_init()
{
	_init_port();
	_out_disable();
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
	rgb = ((rgb << 3) | rgb) & LEDMAT_RGB_ALL;
	memset(LEDMAT_BACK_BUF, rgb, sizeof(LEDMAT_BACK_BUF));
}

void LEDMAT_refresh()
{
	static u8 section;

	_tx_section(LEDMAT_FRONT_BUF[section]);
	
	_out_disable();
	_latch();
	_addr(section);
	_out_enable();
	
	section = RR(section, LEDMAT_SECTION);
}

void LEDMAT_OE_disable()
{
	_out_disable();
}

void LEDMAT_OE_enable()
{
	_out_enable();
}












// UNIT TEST=======================================================================
#if 0

#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect) {
	LMAT_refresh();
}

static void timer0_init() {
	TCCR0 |= _BV(CS01) | _BV(CS00); // ∫–¡÷∫Ò 64
	TIMSK |= _BV(TOIE0);
}

#include <avr/delay.h>

int main() {
	//DDRD |= _BV(7);
	//
	//loop {
		//PORTD ^= _BV(7);
		//_delay_ms(250);
	//}
	
	LMAT_init();
	
	for (u8 i = 0; i < LEDMAT_ROW; i++)
		for (u8 j = 0; j < LEDMAT_COL; j++)
			LMAT_set_rgb_bit(i, j, CR | CG | CB);
	
	timer0_init();
	
	sei();
	
	loop {
		//for (u8 i = 0; i < LEDMAT_H; i++)
		//for (u8 j = 0; j < LEDMAT_W; j++)
		//LMAT_set_rgb(i, j, CR);
//
		//_delay_ms(1000);
//
		//for (u8 i = 0; i < LEDMAT_H; i++)
		//for (u8 j = 0; j < LEDMAT_W; j++)
		//LMAT_set_rgb(i, j, CG | CB);
//
		//_delay_ms(1000);
		
		LMAT_refresh();
		_delay_ms(1);
	}
}

//int main() {
	//LMAT_init();
//
	//
	//// [3, 10], [12, 19]
	//for (u8 i = 0; i < 10; i++) {
		//LMAT_set_rgb(3, 10 + i, CG);
		//LMAT_set_rgb(3 + i, 19, CG);
		//LMAT_set_rgb(12, 10 + i, CG);
		//LMAT_set_rgb(3 + i, 10, CG);
	//}
	//
	//loop {
		//LMAT_refresh();
		//_delay_ms(1);
	//}
//}

//int main(void)
//{
//LED16X32_init_port();
//
//// fill color----------------------------------------
//
////for (uint8_t i = 0; i < 16; i++)
////LED16X32_set_rgb(i, i, (CR | CG | CB) ^ i);
//
////for (uint8_t i = 0; i < 32; i++) {
////LED16X32_set_rgb(0, i, i);
////LED16X32_set_rgb(8, i, ~i);
////}
//
////loop {
////for (uint8_t i = 0; i < 8; i++)
////_tx_section(i, __LED16X32_MATRIX[i]);
////}
//
//// ghosting issue====================================================
//
////for (uint8_t i = 0; i < 32; i += 2) {
////LED16X32_set_rgb(0, i, (CR | CG | CB));
////LED16X32_set_rgb(8, i, (CR | CG | CB));
////LED16X32_set_rgb(3, i + 1, CR);
////LED16X32_set_rgb(11, i + 1, CR);
////}
////
////loop {
////_tx_section(3, __LED16X32_MATRIX[3]);
////_tx_section(0, __LED16X32_MATRIX[0]);
////}
//
//// OE test====================================================
//
//for (uint8_t i = 0; i < 32; i += 2) {
//LED16X32_set_rgb(0, i, (CR | CG | CB));
//LED16X32_set_rgb(8, i, (CR | CG | CB));
//LED16X32_set_rgb(3, i + 1, CR);
//LED16X32_set_rgb(11, i + 1, CR);
//}
//
////loop {
////_out_disable();
////_tx_section(3, __LED16X32_MATRIX[3]);
////_out_enable();
////_delay_ms(1);
////
////
////_out_disable();
////_tx_section(0, __LED16X32_MATRIX[0]);
////_out_enable();
////
////_delay_ms(1);
////}
//
////====================================================
//
////for (uint8_t i = 0; i < 32; i++) {
////LED16X32_set_rgb(0, i, (CR | CG | CB));
////LED16X32_set_rgb(8, i, (CR | CG | CB));
////}
////
////loop {
////_tx_section(0, __LED16X32_MATRIX[0]);
////}
//
//loop {
//LED16X32_refresh();
//_delay_ms(1);
//}
//}

#endif // UNIT TEST