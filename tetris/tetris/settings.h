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
#define LR_SEMICONT_TERM 200UL
#define LR_SEMICONT_CONT 35UL

/* 
+ main() ����
+ S/W debouncing�� ���� INPUT_POLL_MS��
  ���� �ʰ� �����Ѵ�.
*/
#define INPUT_POLL_MS 15UL

#define LEVEL_CHANGE_MS		150UL

// MIN_LVEL�� 0���� ����
#define MAX_LEVEL			16
#define DEFAULT_LEVEL		0

#define LEVEL_BAR_COLOR		(CG | CB)

/* for led matrix refreshing*/ // timer2 OVF
#define TC_REFRESH_CLOCK_SELECT (_BV(CS21) | _BV(CS20)) // 32

/* 1 ~ 255 */
#define MIN_LEDMAT_BRIGHTNESS		1U
#define MAX_LEDMAT_BRIGHTNESS		255U
#define DEFAULT_LEDMAT_BRIGHTNESS	42U

#endif /* SETTINGS_H_ */