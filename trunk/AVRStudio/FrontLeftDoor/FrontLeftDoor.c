/**@file CAN_test.c
* @brief Simple example program for using the MCP2515 CAN interface
* @author Gergo Farkas
* @version 1.0
* @date 2010-11-04
*/
/*****************************************************************************
* Title: Rendszertervezés HF                                                 *
* Hardware: CAN/LIN extension board for mitmót system                        *
* Processor: ATMEGA128                                                       *
* Author: Gergõ Farkas                                                       *
* Date: 2010-10-17 23:24                                                     *
* Compiler: avr-gcc                                                          *
* ------------------------------                                             *
* Description: Simple example program for using the MCP2515 CAN interface    *
******************************************************************************/


/*==============================================================[ INCLUDES ]=*/

#include "..\Common\platform.h"

#include "..\Common\mcp2515.h"
#include "..\Common\dpy_trm_s01.h"
#include "..\Common\TIMER.h"
#include "PRC.h"

/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

/*======================================================[ EXTERNAL GLOBALS ]=*/

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

ISR(SIG_OUTPUT_COMPARE1A);	/**< Timer/Counter1 CompareMatch Interrupt Subroutin*/
int16_t main(void);			/**< The main function*/

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

/*******  20 ms timer1 interrupt  ***********/
ISR(SIG_OUTPUT_COMPARE1A)
{
}


/***************  main  *********************/
int16_t main(void)
{

	_delay_ms(50);

	DPY_v_trm_s01__Init_f(); 							// A kijelzõ panel iniciálása
	CAN_v_mcp2515_init_f();								// A CAN kommunikáció iniciálása
	CAN_v_can_receive_message_ISR_ENABLE_f(); 			// Üzenet fogadás engedélyezve

	TMR_v_timer1_Init_f(50);
	TMR_v_timer1_start_f();

	sei();

	while(1)
	{
		
	}

}

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/


/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/














