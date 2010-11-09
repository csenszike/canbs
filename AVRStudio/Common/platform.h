#ifndef _PLATFORM_H_
#define _PLATFORM_H_


#include "mcu_avr_atmega128_api.h"
#include <stdint.h>
#define AVR_ATMEGA128	1

#define cbi(sfr,bit)    (_SFR_BYTE (sfr) &= ~_BV(bit))
#define sbi(sfr,bit)    (_SFR_BYTE (sfr) |= _BV(bit))

typedef unsigned char bool;
#define true 	1
#define false	0


#endif //_PLATFORM_H_

