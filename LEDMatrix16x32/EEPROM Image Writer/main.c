/*
 * EEPROM Image Writer.c
 *
 * Created: 2022-07-07 12:07:36
 * Author : JMJ
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdint.h>

#define R _BV(0)
#define G _BV(1)
#define B _BV(2)


#if 0
// standby screen

/*
red:		0xff0000ff
orange:		0xff00b4ff
yellow:		0xff00fff0
green:		0xff1eff00
sky-blue:	0xffffe900
blue:		0xffff0800
violet:		0xffff00c3
*/

#define ROW 32
#define COL 16

#define START_ADDR 0
static uint32_t DATA[ROW][COL] = {
#include "standby screen.h"
};

static inline uint8_t to_rgb(uint32_t d) {
	switch(d) {
		case 0xff0000ff: // red
			return R;
		case 0xff00b4ff: // white
			return R | G | B;
		case 0xff00fff0: // yellow
			return R | G;
		case 0xff1eff00: // green
			return G;
		case 0xffffe900: // sky-blue
			return G | B;
		case 0xffff0800: // blue
			return B;
		case 0xffff00c3: // violet
			return R | B;
	}
	
	return 0;
}

int main(void)
{
	uint8_t* addr = (uint8_t*)START_ADDR;
	
    for (uint16_t r = 0; r < ROW; r++) {
		for (uint16_t c = 0; c < COL / 2; c++) {	
			uint8_t rgb = to_rgb(DATA[r][c]);
			rgb |= to_rgb(DATA[r][c + COL / 2]) << 3;

			eeprom_update_byte(addr++, rgb);
		}
	}

}

#endif /* standby screen */

#if 1
// gameover screen

/*
black:		0x00000000
red:		0xff0000ff
green:		0xff00ff00
yellow:		0xff00ffff
blue:		0xffff0000
violet:		0xffff00ff
sky-blue:	0xffffff00
white:		0xffffffff
*/

#define ROW 32
#define COL 16

#define START_ADDR (ROW * (COL / 2))
static uint32_t DATA[ROW][COL] = {
	#include "gameover screen2.h"
};

static inline uint8_t to_rgb(uint32_t d) {
	switch(d) {
		case 0xff0000ff: // red:
			return R;	
		case 0xff00ff00: // green:		
			return G;
		case 0xff00ffff: // yellow:		
			return R | G;
		case 0xffff0000: // blue:		
			return B;
		case 0xffff00ff: // violet:		
			return R | B;
		case 0xffffff00: // sky-blue:	
			return G | B;
		case 0xffffffff: // white:	
			return R | G | B;	
	}
	
	return 0;
}

int main(void)
{
	uint8_t* addr = (uint8_t*)START_ADDR;
	
	for (uint16_t r = 0; r < ROW; r++) {
		for (uint16_t c = 0; c < COL / 2; c++) {
			uint8_t rgb = to_rgb(DATA[r][c]);
			rgb |= to_rgb(DATA[r][c + COL / 2]) << 3;

			eeprom_update_byte(addr++, rgb);
		}
	}

}

#endif /* gameover screen */