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
* Author: Balázs Fuli                                                       *
* Date: 2010-10-17 23:24                                                     *
* Compiler: avr-gcc                                                          *
* ------------------------------                                             *
* Description: Rear Doors process										     *
******************************************************************************/


/*==============================================================[ INCLUDES ]=*/

#include "..\Common\platform.h"

#include "..\Common\mcp2515.h"
#include "..\Common\dpy_trm_s01.h"

/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

#define REARLEFT

#ifdef REARLEFT
	#define L_REA_MSGID 0x122
	#define L_REA_WINUP 5
	#define L_REA_WINDW 4
#endif
#ifdef REARRIGHT
	#define L_REA_MSGID 0x123
	#define L_REA_WINUP 7
	#define L_REA_WINDW 6
#endif
	#define L_REA_ECU 7
	#define L_REA_WIN_ENGINE_UP 6
	#define L_REA_WIN_ENGINE_DOWN 5
	#define L_REA_LOCK 0

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

static volatile bool L_PRC_vbl_status_ECU = false;
static volatile bool L_PRC_vbl_status_win_up = false;
static volatile bool L_PRC_vbl_status_btn_win_up = false;
static volatile bool L_PRC_vbl_status_win_down = false;
static volatile bool L_PRC_vbl_status_btn_win_down = false;
static volatile bool L_PRC_vbl_status_lock = false;


static volatile bool L_PRC_vbl_cmd_win_up = false;
static volatile bool L_PRC_vbl_cmd_win_down = false;
static volatile bool L_PRC_vbl_cmd_lock = false;

/*======================================================[ INTERNAL GLOBALS ]=*/

static volatile CAN_st_message_t L_MAIN_msg_message; /**< Global message*/

/*======================================================[ EXTERNAL GLOBALS ]=*/

CAN_st_message_t PRC_stm_tx_message;

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */


void PRC_v_init_f(void);
void PRC_v_refresh_control_f(void);
void PRC_v_process_f(void);
void PRC_v_refresh_status_f(void);

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/


/***************  main  *********************/
void PRC_v_init_f()
{
	PRC_stm_tx_message.id = L_REA_MSGID;
	PRC_stm_tx_message.rtr = 0;
	PRC_stm_tx_message.length = 2;
	PRC_stm_tx_message.data[0] = 0;
	PRC_stm_tx_message.data[1] = 0;	// Egy CAN üzenet összeállítása
}

void PRC_v_refresh_control_f()
{
	L_PRC_vbl_status_btn_win_up=DPY_TRM_S01__BUTTON_1_GET_STATE();
	L_PRC_vbl_status_btn_win_down=DPY_TRM_S01__BUTTON_2_GET_STATE();

	if(L_PRC_vbl_status_btn_win_up || L_PRC_vbl_status_btn_win_down) //Helyi vezérlés
	{
		if(L_PRC_vbl_status_btn_win_up) L_PRC_vbl_cmd_win_up=true;  //Window Up cmd
		else if(L_PRC_vbl_status_btn_win_down) L_PRC_vbl_cmd_win_down=true; //Window Down cmd
	}
	else //Ha nincs hely vezérlés
	{
		if((CAN_msg_rx_message.data[1]>>L_REA_WINUP)&0x1) //Window Up cmd
		{
			L_PRC_vbl_cmd_win_up=true;
		}
		else
		{
			L_PRC_vbl_cmd_win_up=false; //Ha nincs Window Up cmd
			if((CAN_msg_rx_message.data[1]>>L_REA_WINDW)&0x1) //Window Down cmd
			{
				L_PRC_vbl_cmd_win_down=true;
			}
			else L_PRC_vbl_cmd_win_down=false; //Ha nincs Window Down cmd
		}

	}
	if((CAN_msg_rx_message.data[0]>>L_REA_LOCK)&0x1) //Central lock
	{
		L_PRC_vbl_cmd_lock=true;
	}
	else L_PRC_vbl_cmd_lock=false;
}

void PRC_v_process_f()
{
	if(L_PRC_vbl_cmd_win_up) DPY_TRM_S01__LED_1_ON();
	else DPY_TRM_S01__LED_1_OFF();

	if(L_PRC_vbl_cmd_win_down) DPY_TRM_S01__LED_2_ON();
	else DPY_TRM_S01__LED_2_OFF();

	if(L_PRC_vbl_cmd_lock) DPY_TRM_S01__LED_3_ON();
	else DPY_TRM_S01__LED_3_OFF();
}

void PRC_v_refresh_status_f()
{
	PRC_stm_tx_message.data[0]=(L_PRC_vbl_status_ECU<<L_REA_ECU | 
								L_PRC_vbl_status_win_up<<L_REA_WIN_ENGINE_UP |
								L_PRC_vbl_status_win_down<<L_REA_WIN_ENGINE_DOWN |
								L_PRC_vbl_status_lock<<L_REA_LOCK);
	PRC_stm_tx_message.data[1]=(L_PRC_vbl_status_btn_win_up<<L_REA_ECU | 
								L_PRC_vbl_status_btn_win_down<<L_REA_WIN_ENGINE_UP |
								L_PRC_vbl_status_win_up<<L_REA_WIN_ENGINE_DOWN |
								L_PRC_vbl_status_lock<<L_REA_LOCK);
}

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/


/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/









