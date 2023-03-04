/*
 * timer.c
 *
 * Created: 2022-07-04 12:55:10
 *  Author: JMJ
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint32_t timer0_millis;
static volatile uint16_t timer0_fract;

static inline
void _inc_tick()
{
	uint32_t m = timer0_millis;
	uint16_t f = timer0_fract;
	
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
	uint32_t m;
	
	uint8_t oldSREG = SREG;
	
	cli();
	m = timer0_millis;
	SREG = oldSREG;
	
	return m;
}

ISR(TIMER0_OVF_vect)
{
	_inc_tick();
}