/**@file CAN_test.c
* @brief Simple example program for using the MCP2515 CAN interface
* @author Gergo Farkas
* @version 1.0
* @date 2010-11-04
*/
/*****************************************************************************
* Title: Rendszertervez�s HF                                                 *
* Hardware: CAN/LIN extension board for mitm�t system                        *
* Processor: ATMEGA128                                                       *
* Author: Gerg� Farkas                                                       *
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

/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

static volatile CAN_st_message_t L_MAIN_msg_message; /**< Global message*/

/*======================================================[ EXTERNAL GLOBALS ]=*/

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

ISR(SIG_OUTPUT_COMPARE1A);	/**< Timer/Counter1 CompareMatch Interrupt Subroutin*/
int16_t main(void);			/**< The main function*/

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

/*******  20 ms timer1 interrupt  ***********/
ISR(SIG_OUTPUT_COMPARE1A)
{
//	L_MAIN_msg_message.data[0]++;
	CAN_v_can_send_standard_message_f(&L_MAIN_msg_message);	// CAN �zenetk�ld�s
}


/***************  main  *********************/
int16_t main(void)
{

	_delay_ms(50);

	DPY_v_trm_s01__Init_f(); 							// A kijelz� panel inici�l�sa
	CAN_v_mcp2515_init_f();								// A CAN kommunik�ci� inici�l�sa
	CAN_v_can_receive_message_ISR_ENABLE_f(); 			// �zenet fogad�s enged�lyezve

	TMR_v_timer1_Init_f(50);
	TMR_v_timer1_start_f();

	L_MAIN_msg_message.id = 0x0123;
	L_MAIN_msg_message.rtr = 0;
	L_MAIN_msg_message.length = 2;
	L_MAIN_msg_message.data[0] = 0;
	L_MAIN_msg_message.data[1] = 0;	// Egy CAN �zenet �ssze�ll�t�sa

	sei();

	while(1)
	{
		
		// Ha van �j �zenet
		if(CAN_vbl_New_message_flag){
			cli();
			CAN_vbl_New_message_flag=false;
//			DPY_u8_trm_s01__7seq_write_number_f(CAN_msg_rx_message.data[0], 0);
			if(CAN_msg_rx_message.id == 0x0100)
				L_MAIN_msg_message.data[0] = CAN_msg_rx_message.data[0];
			else if(CAN_msg_rx_message.id == 0x01E0)
				L_MAIN_msg_message.data[1] = CAN_msg_rx_message.data[0];
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














