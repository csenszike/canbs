
/*==============================================================[ INCLUDES ]=*/

#include "platform.h"

#include "PRC.h"


/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/
// 0x121
// Byte 0
static volatile bool L_PRC_vbl_ECU_status= false;
static volatile bool L_PRC_vbl_win_engine_up_status= false;
static volatile bool L_PRC_vbl_win_engine_down_status= false;
static volatile bool L_PRC_vbl_lock_status= false;
// Byte 1
static volatile bool L_PRC_vbl_win_engine_up_button_status= false;
static volatile bool L_PRC_vbl_win_engine_down_button_status= false;

// Byte 2
static volatile bool L_PRC_vbl_win_lock_state_control= false;
static volatile bool L_PRC_vbl_win_lock_button= false;


// 0x120
// Byte 0

// Byte 1
static volatile bool L_PRC_vbl_win_engine_up_control= false;
static volatile bool L_PRC_vbl_win_engine_down_control= false;
// Byte 2
static volatile bool L_PRC_vbl_win_mirror_left_control= false;
static volatile bool L_PRC_vbl_win_mirror_right_control= false;
static volatile bool L_PRC_vbl_win_mirror_up_control= false;
static volatile bool L_PRC_vbl_win_mirror_down_control= false;
static volatile bool L_PRC_vbl_win_mirror_select_control= false;
static volatile bool L_PRC_vbl_win_mirror_heat_control= false;
static volatile bool L_PRC_vbl_win_lock_state_control= false;
static volatile bool L_PRC_vbl_win_lock_button= false;
static volatile bool L_PRC_vbl_win_lock_state_control= false;
static volatile bool L_PRC_vbl_win_lock_button= false;


/*======================================================[ EXTERNAL GLOBALS ]=*/
CAN_st_message_t PRC_stm_tx_message;

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

void PRC_v_init_f(void)
{
	PRC_stm_tx_message.id = 0x0121;
	PRC_stm_tx_message.rtr = 0;
	PRC_stm_tx_message.length = 3;
	PRC_stm_tx_message.data[0] = 0;
	PRC_stm_tx_message.data[1] = 0;	
	PRC_stm_tx_message.data[2] = 0; // Egy CAN üzenet összeállítása
}

void PRC_v_refresh_control_f(void)
{
	CAN_msg_rx_message

}

void PRC_v_process_f(void)
{

}

void PRC_v_refresh_status_f(void)
{
	PRC_stm_tx_message;
}



/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/

