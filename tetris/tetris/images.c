/*
 * images.c
 *
 * Created: 2022-10-10 15:40:34
 *  Author: JMJ
 */

#include <avr/pgmspace.h>

const uint8_t GAMEOVER_IMAGE[32][8] PROGMEM = {
	{0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,},
	{0x3f,0x9,0x39,0x9,0x3f,0xf,0x9,0xf,},
	{0x3f,0x9,0xf,0xf,0x3f,0x9,0x3f,0x39,},
	{0x3f,0x9,0x3f,0x9,0x3f,0x9,0xf,0x9,},
	{0x3f,0x9,0x3f,0x9,0x3f,0x9,0x39,0x39,},
	{0x3f,0x9,0x39,0x9,0x3f,0x9,0xf,0x9,},
	{0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,},
	{0xf,0x9,0xf,0x3f,0x9,0xf,0x9,0x3f,},
	{0x9,0x3f,0x39,0x3f,0x9,0x3f,0x39,0xf,},
	{0x9,0xf,0x9,0x3f,0x9,0xf,0x9,0x3f,},
	{0x9,0x3f,0x39,0x3f,0x9,0x3f,0x9,0x3f,},
	{0xf,0x9,0xf,0x3f,0xf,0x39,0x3f,0xf,},
	{0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
};

const uint8_t STANDBY_IMAGE[32][8] PROGMEM = {
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x38,0x38,0x1,0x19,0x19,0x18,0x0,0x7,},
	{0x0,0x0,0x0,0x1,0x18,0x0,0x0,0x7,},
	{0x38,0x38,0x0,0x1,0x18,0x0,0x0,0x7,},
	{0x0,0x0,0x0,0x1,0x18,0x0,0x0,0x7,},
	{0x38,0x38,0x0,0x1,0x18,0x0,0x0,0x7,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x30,0x30,0x2,0x2a,0x2a,0x28,0x0,0x6,},
	{0x30,0x0,0x2,0x28,0x0,0x2,0x0,0x0,},
	{0x30,0x0,0x2,0x2a,0x2a,0x28,0x0,0x0,},
	{0x30,0x0,0x2,0x0,0x2,0x28,0x0,0x0,},
	{0x30,0x30,0x2,0x28,0x28,0x2a,0x0,0x6,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
	{0x0,0x0,0x10,0x0,0x0,0x0,0x0,0x30,},
	{0x0,0x0,0x10,0x11,0x39,0x0,0x0,0x30,},
	{0x3,0x23,0x20,0x17,0x39,0x39,0x38,0x35,},
	{0x2b,0x23,0x28,0x2f,0x2f,0xf,0xd,0x35,},
	{0xc,0x26,0x16,0x2e,0x26,0x22,0xa,0x9,},
	{0xc,0xc,0x14,0x15,0x22,0x3a,0x3b,0x3b,},
	{0x29,0x29,0x29,0x15,0x25,0x39,0x1b,0x1b,},
	{0x3b,0x2b,0x21,0x25,0x9,0x11,0x1f,0x1f,},
	{0x1b,0x1b,0x27,0xa,0x9,0x15,0x17,0x24,},
	{0x1f,0x1f,0x27,0xa,0x1a,0x1d,0x15,0x24,},
	{0x36,0x36,0x36,0x36,0x1a,0x1d,0x24,0x24,},

};