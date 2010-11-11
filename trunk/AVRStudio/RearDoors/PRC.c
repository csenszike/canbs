/**@file PRC.c
* @brief Rear Doors process program
* @author Balazs Fuli
* @version 1.1
* @date 2010-11-11
*/
/*****************************************************************************
* Title: Rendszertervezés HF                                                 *
* Hardware: CAN/LIN extension board for mitmót system                        *
* Processor: ATMEGA128                                                       *
* Author: Balázs Fuli                                                       *
* Date:                                                    *
* Compiler: avr-gcc                                                          *
* ------------------------------                                             *
* Description: Rear Doors process										     *
******************************************************************************/


/*==============================================================[ INCLUDES ]=*/

#include "..\Common\platform.h"

#include "PRC.h"
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

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/**
 *static volatile bool L_PRC_vbl_status_ECU = false;
 *static volatile bool L_PRC_vbl_status_win_up = false;
 *static volatile bool L_PRC_vbl_status_btn_win_up = false;
 *static volatile bool L_PRC_vbl_status_win_down = false;
 *static volatile bool L_PRC_vbl_status_btn_win_down = false;
 *static volatile bool L_PRC_vbl_status_lock = false;
 *
 *
 *static volatile bool L_PRC_vbl_cmd_win_up = false;
 *static volatile bool L_PRC_vbl_cmd_win_down = false;
 *static volatile bool L_PRC_vbl_cmd_lock = false;
 */


/*======================================================[ INTERNAL GLOBALS ]=*/

/* Left front door control bits  (0x120)*/
// Byte 0
static volatile bool L_PRC_vbl_win_engine_up_control= false;
static volatile bool L_PRC_vbl_win_engine_down_control= false;
// Byte 1
static volatile bool L_PRC_vbl_lock_frontleftbutton= false;

/* Left front door status bits  (0x120)*/
// Byte 3
static volatile bool L_PRC_vbl_lock_frontleftstatus= false;

/* Own control bits (0x121)*/
// Byte 0
static volatile bool L_PRC_vbl_win_engine_up_rearbutton= false;
static volatile bool L_PRC_vbl_win_engine_down_rearbutton= false;

/* Own status bits (0x121)*/
// Byte 1
static volatile bool L_PRC_vbl_win_engine_up_status= false;
static volatile bool L_PRC_vbl_win_engine_down_status= false;
static volatile bool L_PRC_vbl_lock_rearstatus= false;
// Byte 2
static volatile bool L_PRC_vbl_ECU_status= false;

/*======================================================[ EXTERNAL GLOBALS ]=*/

CAN_st_message_t PRC_stm_tx_message;

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

static bool L_PRC_bl_get_msg_byte_bit_f(uint8_t byte,uint8_t bit);
static void L_PRC_bl_set_msg_byte_bit_f(uint8_t byte,uint8_t bit,bool value);

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

static bool L_PRC_bl_get_msg_byte_bit_f(uint8_t byte,uint8_t bit)
{
	return ((CAN_msg_rx_message.data[byte]>>bit)&0x01);
}
static void L_PRC_bl_set_msg_byte_bit_f(uint8_t byte,uint8_t bit,bool value)
{
	if(value)
		PRC_stm_tx_message.data[byte] |= (1<<bit);
	else
		PRC_stm_tx_message.data[byte] &= ~(1<<bit);
}


/

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

/********  Initialisation  ***************************/
void PRC_v_init_f(void)
{
	PRC_stm_tx_message.id = L_REA_MSGID;
	PRC_stm_tx_message.rtr = 0;
	PRC_stm_tx_message.length = 3;
	PRC_stm_tx_message.data[0] = 0;
	PRC_stm_tx_message.data[1] = 0;	
	PRC_stm_tx_message.data[2] = 0; // Egy CAN üzenet összeállítása
}


void PRC_v_refresh_control_f()
{
	L_PRC_vbl_win_engine_up_rearbutton=  DPY_TRM_S01__BUTTON_1_GET_STATE();
	L_PRC_vbl_win_engine_down_rearbutton=DPY_TRM_S01__BUTTON_2_GET_STATE();
	
	L_PRC_vbl_win_engine_up_control=    L_PRC_bl_get_msg_byte_bit_f(0,L_REA_WINUP);
	L_PRC_vbl_win_engine_down_control=  L_PRC_bl_get_msg_byte_bit_f(0,L_REA_WINDW);
	L_PRC_vbl_lock_frontleftbutton=          L_PRC_bl_get_msg_byte_bit_f(1,0);
	
	L_PRC_vbl_lock_frontleftstatus= 			L_PRC_bl_get_msg_byte_bit_f(4,7);
}

void PRC_v_process_f()
{
	/************** ABLAK ****************/
	// Ablak vezérlése a bal elsõ ajtóról  (amíg nyomva tartjuk a gombot, addig mozog az ablak)
	if(L_PRC_vbl_win_engine_up_control || L_PRC_vbl_win_engine_down_control)
	{
		if(L_PRC_vbl_win_engine_up_control){     		//Window Up 
			L_PRC_vbl_win_engine_down_status=false;	
			L_PRC_vbl_win_engine_up_status=true;
		}	 
		else if(L_PRC_vbl_win_engine_down_control){ 	//Window Down
			L_PRC_vbl_win_engine_down_status=true;
			L_PRC_vbl_win_engine_up_status=false;
		} 
	}
	// Ablak helyi vezérlése (amíg nyomva tartjuk a gombot, addig mozog az ablak)
	else if(L_PRC_vbl_win_engine_up_rearbutton || L_PRC_vbl_win_engine_down_rearbutton)
	{
		if(L_PRC_vbl_win_engine_up_rearbutton){        //Window Up 
			L_PRC_vbl_win_engine_down_status=false;	
			L_PRC_vbl_win_engine_up_status=true;
		}	 
		else if(L_PRC_vbl_win_engine_down_rearbutton){  //Window Down
			L_PRC_vbl_win_engine_down_status=true;
			L_PRC_vbl_win_engine_up_status=false;
		}  
	}
	else{
		L_PRC_vbl_win_engine_down_status=false;
		L_PRC_vbl_win_engine_up_status=false;
	}

	/************** KÖZPONTI ZÁR ****************/
	// Központi zár vezérlése a bal ajtóról  (ha megnyomjuk a gombot a központi zár állapota megváltozik)
	if(L_PRC_vbl_lock_frontleftbutton)
	{
		L_PRC_vbl_lock_rearstatus=L_PRC_vbl_lock_frontleftstatus;	
	}

	/************** VEZÉRLÉS ****************/
	if(L_PRC_vbl_win_engine_up_status) DPY_TRM_S01__LED_1_ON();
	else DPY_TRM_S01__LED_1_OFF();

	if(L_PRC_vbl_win_engine_down_status) DPY_TRM_S01__LED_2_ON();
	else DPY_TRM_S01__LED_2_OFF();

	if(L_PRC_vbl_lock_rearstatus) DPY_TRM_S01__LED_3_ON();
	else DPY_TRM_S01__LED_3_OFF();
}

void PRC_v_refresh_status_f()
{
	L_PRC_bl_set_msg_byte_bit_f(0,L_REA_WINUP,L_PRC_vbl_win_engine_up_rearbutton);
	L_PRC_bl_set_msg_byte_bit_f(0,L_REA_WINDW,L_PRC_vbl_win_engine_down_rearbutton);
	L_PRC_bl_set_msg_byte_bit_f(1,7,L_PRC_vbl_win_engine_up_status);
	L_PRC_bl_set_msg_byte_bit_f(1,6,L_PRC_vbl_win_engine_down_status);
	L_PRC_bl_set_msg_byte_bit_f(1,0,L_PRC_vbl_lock_rearstatus);
	L_PRC_bl_set_msg_byte_bit_f(0,7,L_PRC_vbl_ECU_status);

}

/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/









