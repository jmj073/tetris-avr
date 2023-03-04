/*
 * timer.h
 *
 * Created: 2022-07-04 12:54:58
 *  Author: JMJ
 */ 


#ifndef TIMER_H_
#define TIMER_H_


#include <stdint.h>

#if 1 /* first version */
#define TIME_OUTA(curr, prev, n)(((curr) - (prev)) >= n\
								&& ((prev) = (curr), 1))
#define TIME_OUTI(curr, prev, n)(((curr) - (prev)) >= n\
								&& ((prev) += n, 1))

#define DEF_PREV_MS(n) uint32_t __prev_ms_##n
#define PREV_MS(n) __prev_ms_##n
// Assignment
#define TIME_OUT_MSA(curr, n) (((curr) - __prev_ms_##n) >= n\
								&& (__prev_ms_##n = (curr), 1))
// Increase
#define TIME_OUT_MSI(curr, n) (((curr) - __prev_ms_##n) >= n\
								&& (__prev_ms_##n += n, 1))

#define DEF_PREV_US(n) uint32_t __prev_us_##n
#define PREV_US(n) __prev_us_##n
// Assignment
#define TIME_OUT_USA(curr, n) (((curr) - __prev_us_##n) >= n\
								&& (__prev_us_##n = (curr), 1))
// Increase
#define TIME_OUT_USI(curr, n) (((curr) - __prev_us_##n) >= n\
								&& (__prev_us_##n += n, 1))
#endif /* first version */

#if 1 /* second version */

#endif /* second version */


/* 
+ TimeBase: timer/counter 0
+ ticker: timer/counter 3
*/

void TimeBase_init();
void TimeBase_inc_tick();

uint32_t millis();


#endif /* TIMER_H_ */