/*
 * settings.h
 *
 * Created: 2022-07-03 10:46:59
 *  Author: JMJ
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <avr/io.h>

/* 
## semi-continuous(반연속) 
+ **반연속**: 버튼 입력 방식 중의 하나.
	예를 들자면, 키보드로 a를 입력시 a 한글자가 나오고 
	조금의 텀이 있은 후에 연속으로 a가 나오는 것을
	예로 들 수 있다.
	
*/
#define LR_SEMICONT_TERM 500
#define LR_SEMICONT_CONT 75

#define INPUT_POLL_MS 15
#define TICK_MS 300
#define BtN_LR_MS 200

/* for led matrix refreshing*/ // timer2 OVF
#define TC_REFRESH_CLOCK_SELECT (_BV(CS21) | _BV(CS20)) // 32
// using 8-bit Timer/Counter OVF interrupt

#endif /* SETTINGS_H_ */