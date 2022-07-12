/*
 * pins.h
 *
 * Created: 2022-07-03 14:47:52
 *  Author: JMJ
 */ 


#ifndef PINS_H_
#define PINS_H_

#include <avr/io.h>
#include "my_util.h"

/* Set Bit */
#define _SB(x, pin, bit) ((void)(x(pin) |= _BV(bit)))
/* Clear Bit */
#define _CB(x, pin, bit) ((void)(x(pin) &= ~_BV(bit)))

#define DEF_PREV_DIGITAL_READ(n) u8 __PREV_DIGITAL_READ##n
#define IS_RISING(n, curr, bit) ((bit) & (curr) & ~__PREV_DIGITAL_READ##n)
#define IS_FALLING(n, curr, bit) ((bit) & ~(curr) & __PREV_DIGITAL_READ##n)
#define IS_HIGH(curr, bit) ((bit) & (curr))
#define IS_LOW(curr, bit) ((bit) & ~(curr))
#define UPDATE_PREV_DIGITAL_READ(n, curr) ((void)(__PREV_DIGITAL_READ##n = (curr)))

/*
	+ 핀 매핑 설정을 위해 추가적으로 봐야할 파일:
		+ led_matrix_16x32.h
*/

#define BtN_PIN (&PINA)
#define BtN_ALL_PINS 0x0F
#define BtN_UP		0
#define BtN_DOWN	1
#define BtN_LEFT	2
#define BtN_RIGHT	3
#define BtN_PRESSED() (~PIN(BtN_PIN) & BtN_ALL_PINS)


#endif /* PINS_H_ */