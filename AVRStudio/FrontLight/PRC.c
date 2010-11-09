
/*==============================================================[ INCLUDES ]=*/

#include "platform.h"

#include "TIMER.h"


/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

volatile uint8_t TMR_u8_OST_cnt = 0;

/*======================================================[ EXTERNAL GLOBALS ]=*/

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

/********  Timer0 initialisation  ***************************/
void TMR_v_timer0_Init_f(void)
{
	TCCR0 = (1<<CS02);//(1<<CS02);			//8000000/64/256 = 488.28 Hz => 2 ms
}

/********  Timer1 initialisation  ***************************/
void TMR_v_timer1_Init_f(uint16_t freq)
{
	TCCR1A = 0;
	TCCR1B = (1<<CS12)|(1<<WGM12);		//CLK/256, CTC
	OCR1A = 31250/freq-1;				//8000000/256 = 31250
}

/********  Timer1 initialisation  ***************************/
void TMR_v_timer1_start_f(void)
{
	sbi(TIMSK,OCIE1A);
}

/********  Timer1 initialisation  ***************************/
void TMR_v_timer1_stop_f(void)
{
	sbi(TIFR,OCF1A);
	cbi(TIMSK,OCIE1A);
}

/********  Timer3 initialisation  ***************************/
void TMR_v_timer3_Init_f(uint16_t freq) 
{
	TCCR3A = 0;
	TCCR3B = (1<<CS32)|(1<<WGM32);		//CLK/256, CTC
	OCR3A = 31250/freq-1;				//8000000/256 = 31250
	sbi(ETIMSK,OCIE3A);
}
/********  start OST  ***************************/
void TMR_v_start_OST_f(uint16_t ms)
{
	TMR_u8_OST_cnt = ms/2;
	TCNT0 = 0;
	sbi(TIFR,TOV0);
    sbi(TIMSK,TOIE0);
}

/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/

