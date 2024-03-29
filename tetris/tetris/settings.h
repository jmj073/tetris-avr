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
#define LR_SEMICONT_TERM 200UL
#define LR_SEMICONT_CONT 35UL

/* 
+ main() 참고
+ S/W debouncing을 위해 INPUT_POLL_MS는
  작지 않게 유지한다.
*/
#define INPUT_POLL_MS 15UL

#define LEVEL_CHANGE_MS		150UL

// MIN_LVEL은 0으로 고정
#define MAX_LEVEL			16
#define DEFAULT_LEVEL		0

#define LEVEL_BAR_COLOR		(CG | CB)



#endif /* SETTINGS_H_ */