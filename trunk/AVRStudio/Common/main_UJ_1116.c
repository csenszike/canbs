/**@file CAN_test.c
* @brief Simple example program for using the MCP2515 CAN interface
* @author Gabor Fodor
* @version 1.0
* @date 2010-11-04
*/
/*****************************************************************************
* Title: Rendszertervezés HF                                                 *
* Hardware: CAN/LIN extension board for mitmót system                        *
* Processor: ATMEGA128                                                       *
* Author: Gábor Fodor                                                       *
* Date:                                                      *
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

volatile bool CAN_vbl_20msTimer_flag=false;
volatile uint16_t CAN_vu16_20msTimer_cnt=0;

/*======================================================[ EXTERNAL GLOBALS ]=*/

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

ISR(SIG_OUTPUT_COMPARE1A);	/**< Timer/Counter1 CompareMatch Interrupt Subroutin*/
int16_t main(void);			/**< The main function*/

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

/*******  20 ms timer1 interrupt  ***********/
ISR(SIG_OUTPUT_COMPARE1A)
{
	
	PRC_v_refresh_message_status_f();
	//System tick a PRC-nek
	PRC_v_20mstick_f();
	CAN_vu16_20msTimer_cnt+=1;
	if (CAN_vu16_20msTimer_cnt==PRC_U16_SENDX20MS)
	{
		CAN_v_can_send_standard_message_f(&PRC_stm_tx_message);	// CAN üzenetküldés
		CAN_vu16_20msTimer_cnt=0;
	}
	CAN_vbl_20msTimer_flag=true;
}


/***************  main  *********************/
int16_t main(void)
{

	_delay_ms(50);

	DPY_v_trm_s01__Init_f(); 							// A kijelzõ panel iniciálása
	CAN_v_mcp2515_init_f(PRC_U16_FILTER1, PRC_U16_FILTER2);								// A CAN kommunikáció iniciálása
	CAN_v_can_receive_message_ISR_ENABLE_f(); 			// Üzenet fogadás engedélyezve

	TMR_v_timer1_Init_f(50);
	TMR_v_timer1_start_f();

	PRC_v_init_f();										//Feladatspecifikus inicializációk elvégzése
	sei();

	while(1)
	{
		
		// Ha van új üzenet
		if(CAN_vbl_New_message_flag){
			CAN_vbl_New_message_flag=false;
			cli();
			PRC_v_refresh_remote_control_f();
			sei();
			
		}
		if(CAN_vbl_20msTimer_flag)
		{
			CAN_vbl_20msTimer_flag=false;
			PRC_v_refresh_local_control_f();
			PRC_v_process_f();
			PRC_v_refresh_local_status_f();
		}
	}

}

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/


/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/
