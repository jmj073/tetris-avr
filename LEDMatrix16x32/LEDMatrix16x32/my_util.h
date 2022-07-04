/*
 * my_util.h
 *
 * Created: 2022-06-30 10:24:24
 * Author : JMJ
 * Version: 1.0
 */ 

#ifndef MY_UTIL_H
#define MY_UTIL_H

// bit================================================

/* right rotate shift */
// WARNING: 'a' must be unsigned
#define rrs(d, n) \
({ \
	_Static_assert((typeof(d))-1 >= 0, \
	"argument 'd' to 'rrs(d, n) macro' must be unsigned"); \
	typeof(d) _d = (d); \
	typeof(n) _n = (n) & 7; \
	( (_d << (sizeof(_d) * 8 - _n)) | (_d >> _n) ); \
})

/* left rotate shift */
#define lrs(d, n) \
({ \
	_Static_assert((typeof(d))-1 >= 0, \
	"argument 'd' to 'lrs(d, n) macro' must be unsigned"); \
	typeof(d) _d = (d); \
	typeof(n) _n = (n) & 7; \
	( (_d >> (sizeof(_d) * 8 - _n)) | (_d << _n) ); \
})

// control statement==================================

#define loop for(;;)
#define check_ret(bool, ...) if (bool) return __VA_ARGS__

// PIN================================================

/*
+ NOTE: The argument 'pin' must be &PINX(X=A,B,C,D,E,G)
+ WARNING:
	'pin' cannot be &PINF(at least in one ATmega128A)
	This is because PINF is not in the order of PIN, DDR, and PORT.
+ example: DDR(&PINA) => DDRA
*/
#define PIN(pin)	((pin)[0])
#define DDR(pin)	((pin)[1])
#define PORT(pin)	((pin)[2])

// etc================================================

/* 
**for software debouncing**

	+ arguments:
		source, flag, delay

	+ example:
		double_check(PINB, 0x01, 20)

*/
#define double_check(s, f, delay) (\
	((s) & (f)) &&\
	( _delay_ms(delay), ((s) & (f)) )\
)

#define min(a, b) \
({ \
	typeof(a) _a = (a); \
	typeof(b) _b = (b); \
	(_a < _b ? _a : _b); \
})

#define round_align(dst, divisor) \
({ \
	typeof(dst) a = (dst); \
	typeof(divisor) b = (divisor); \
	((a + b - 1) / b) * b; \
})

/* right rotate */
#define RR(d, n) (((d) + 1) % (n))
/* left rotate */
#define LR(d, n)\
({\
	typeof(n) _n = (n);\
	((d) + _n - 1) % _n;\
})


#endif // MY_UTIL_H