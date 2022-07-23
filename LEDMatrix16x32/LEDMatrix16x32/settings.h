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
## semi-continuous(�ݿ���) 
+ **�ݿ���**: ��ư �Է� ��� ���� �ϳ�.
	���� ���ڸ�, Ű����� a�� �Է½� a �ѱ��ڰ� ������ 
	������ ���� ���� �Ŀ� �������� a�� ������ ����
	���� �� �� �ִ�.
*/
#define LR_SEMICONT_TERM 300
#define LR_SEMICONT_CONT 60

/* 
+ main() ����
+ S/W debouncing�� ���� INPUT_POLL_MS��
  ���� �ʰ� �����Ѵ�.
*/
#define INPUT_POLL_MS 15
#define TICK_MS 300

/* for led matrix refreshing*/ // timer2 OVF
// 32
#define TC_REFRESH_CLOCK_SELECT (_BV(CS21) | _BV(CS20))

/* 1 ~ 255 */
#define MIN_LEDMAT_BRIGHTNESS		1U
#define MAX_LEDMAT_BRIGHTNESS		255U
#define DEFAULT_LEDMAT_BRIGHTNESS	42U


/*
ifdef LAND_CONTROL => DOWN KEY: land piece
else  => DONW KEY: left rotate piece
*/
//#define LAND_CONTROL

#endif /* SETTINGS_H_ */