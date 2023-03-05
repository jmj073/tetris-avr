/*
 * timer.c
 *
 * Created: 2022-07-04 12:55:10
 *  Author: JMJ
 */

#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static volatile u32 timer0_millis;
static volatile u16 timer0_fract;

static inline
void _inc_tick()
{
	u32 m = timer0_millis;
	u16 f = timer0_fract;
	
	m++, f += 24;
	if (f >= 1000) m++, f -= 1000;
	
	timer0_millis = m;
	timer0_fract = f;
}

void TimeBase_init()
{
	TCCR0 |= _BV(CS02); // clock select: 64
	TIMSK |= _BV(TOIE0); // overflow interrupt enable
}

uint32_t millis()
{
	u32 m;
	
	u8 oldSREG = SREG;
	
	cli();
	m = timer0_millis;
	SREG = oldSREG;
	
	return m;
}

ISR(TIMER0_OVF_vect)
{
	_inc_tick();
}