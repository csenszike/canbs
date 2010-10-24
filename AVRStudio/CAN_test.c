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

#include "platform.h"

#include "mcp2515.h"
#include "dpy_trm_s01.h"
#include "TIMER.h"

/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

static CAN_st_message_t L_MAIN_msg_message;

/*======================================================[ EXTERNAL GLOBALS ]=*/

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

ISR(SIG_OUTPUT_COMPARE1A);
int16_t main(void);

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

/*******  20 ms timer1 interrupt  ***********/
ISR(SIG_OUTPUT_COMPARE1A)
{
	L_MAIN_msg_message.data[0]++;
	CAN_v_can_send_standard_message_f(&L_MAIN_msg_message);	// CAN üzenetküldés
}


/***************  main  *********************/
int16_t main(void)
{

	_delay_ms(50);

	DPY_v_trm_s01__Init_f(); 							// A kijelzõ panel iniciálása
	CAN_v_mcp2515_init_f();								// A CAN kommunikáció iniciálása
	CAN_v_mcp2515_Set_standard_filter_RxF0_f(0x0100);	// Filter beállítása???
	CAN_v_can_receive_message_ISR_ENABLE_f(); 			// Üzenet fogadás engedélyezve

	TMR_v_timer1_Init_f(50);
	TMR_v_timer1_start_f();

	L_MAIN_msg_message.id = 0x0123;
	L_MAIN_msg_message.rtr = 0;
	L_MAIN_msg_message.length = 2;
	L_MAIN_msg_message.data[0] = 0;
	L_MAIN_msg_message.data[1] = 0;	// Egy CAN üzenet összeállítása

	sei();

	while(1)
	{
		
		// Ha van új üzenet
		if(CAN_vbl_New_message_flag){
			cli();
			CAN_vbl_New_message_flag=false;
			DPY_u8_trm_s01__7seq_write_number_f(CAN_msg_rx_message.data[2], 0);
			sei();
		}
	}

}

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/


/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/














