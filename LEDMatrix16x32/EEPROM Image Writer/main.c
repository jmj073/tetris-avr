/*
 * EEPROM Image Writer.c
 *
 * Created: 2022-07-07 12:07:36
 * Author : JMJ
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdint.h>

/*

red:		0xff0000ff
orange:		0xff00b4ff
yellow:		0xff00fff0
green:		0xff1eff00
sky-blue:	0xffffe900
blue:		0xffff0800
violet:		0xffff00c3

*/

#define START_ADDR 0

#define ROW 32
#define COL 16


#define R _BV(0)
#define G _BV(1)
#define B _BV(2)

static uint32_t DATA[32][16] = {
#include "matrix.h"
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