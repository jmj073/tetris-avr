/*
 * timer.h
 *
 * Created: 2022-07-04 12:54:58
 *  Author: JMJ
 */ 


#ifndef TIMER_H_
#define TIMER_H_


#include "shortint.h"

static inline u8 TIME_OUTA(u32 curr, u32* prev, u32 n) {
	return (curr - (*prev)) >= n && ((*prev) = curr, 1);
}
static inline u8 TIME_OUTI(u32 curr, u32* prev, u32 n) {
	return (curr - (*prev)) >= n && ((*prev) += n, 1);
}

#define DEF_PREV_MS(n) u32 __prev_ms_##n
#define PREV_MS(n) __prev_ms_##n
// Assignment
#define TIME_OUT_MSA(curr, n) TIME_OUTA(curr, &(__prev_ms_##n), n)
// Increase
#define TIME_OUT_MSI(curr, n) TIME_OUTI(curr, &(__prev_ms_##n), n)

#define DEF_PREV_US(n) u32 __prev_us_##n
#define PREV_US(n) __prev_us_##n
// Assignment
#define TIME_OUT_USA(curr, n) TIME_OUTA(curr, &(__prev_us_##n), n)
// Increase
#define TIME_OUT_USI(curr, n) TIME_OUTI(curr, &(__prev_us_##n), n)


/* 
+ TimeBase: timer/counter 0
*/

void TimeBase_init();

u32 millis();


#endif /* TIMER_H_ */