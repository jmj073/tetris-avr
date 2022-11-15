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

/*
 *	+ 핀 매핑 설정을 위해 추가적으로 봐야할 파일:
 *		+ led_matrix_16x32.h
 */

#define BTN_PIN (&PIND)
#define BTN_ALL_PINS 0x0F
#define BTN_UP		0
#define BTN_DOWN	1
#define BTN_LEFT	2
#define BTN_RIGHT	3
#define BTN_PRESSED() (~PIN(BTN_PIN) & BTN_ALL_PINS)


#endif /* PINS_H_ */