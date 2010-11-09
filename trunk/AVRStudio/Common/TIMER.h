/*

	ADC driver

*/


/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*===========================================================[ HEADER INIT ]=*/

#ifndef TIMER_H
#define TIMER_H


/*=======================================================[ EXTERNAL MACROS ]=*/
/* macro constants and read macros */

/*=====================================================[ EXTERNAL TYPEDEFS ]=*/
/* State Machine Constants */


/*======================================================[ EXTERNAL GLOBALS ]=*/
/* Just for debug !!!!*/
extern volatile uint8_t TMR_u8_OST_cnt;

/*=========================================[ EXTPORTED FUNCTION PROTOTYPES ]=*/

extern void TMR_v_timer0_Init_f(void);
extern void TMR_v_timer1_Init_f(uint16_t);
extern void TMR_v_timer1_start_f(void);
extern void TMR_v_timer1_stop_f(void);
extern void TMR_v_timer3_Init_f(uint16_t);
extern void TMR_v_start_OST_f(uint16_t);


#endif /* TIMER_H */
/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/

