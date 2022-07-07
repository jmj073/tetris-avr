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
#define LR_SEMICONT_TERM 500
#define LR_SEMICONT_CONT 75

#define INPUT_POLL_MS 15
#define TICK_MS 300
#define BtN_LR_MS 200

/* for led matrix refreshing*/ // timer2 OVF
#define TC_REFRESH_CLOCK_SELECT (_BV(CS21) | _BV(CS20)) // 32
// using 8-bit Timer/Counter OVF interrupt

#endif /* SETTINGS_H_ */