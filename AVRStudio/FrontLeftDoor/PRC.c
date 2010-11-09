
/*==============================================================[ INCLUDES ]=*/

#include "..\Common\platform.h"

#include "..\Common\mcp2515.h"

#include "PRC.h"


/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

/* Right rear door control bits */
/* Byte 2 */
static volatile bool L_PRC_vbl_RRD_lock_button_pressed	= false;

/* Own status and control bits */
/* Byte 0 */
static volatile bool L_PRC_vbl_ECU_stat					= false;
static volatile bool L_PRC_vbl_window_motor_up_stat		= false;
static volatile bool L_PRC_vbl_window_motor_down_stat	= false;
static volatile bool L_PRC_vbl_lock_stat				= false;

/* Byte 1 */
static volatile bool L_PRC_vbl_right_rear_window_up		= false;
static volatile bool L_PRC_vbl_right_rear_window_down	= false;
static volatile bool L_PRC_vbl_left_rear_window_up		= false;
static volatile bool L_PRC_vbl_left_rear_window_down	= false;
static volatile bool L_PRC_vbl_right_front_window_up	= false;
static volatile bool L_PRC_vbl_right_front_window_down	= false;
static volatile bool L_PRC_vbl_left_front_window_up		= false;
static volatile bool L_PRC_vbl_left_front_window_down	= false;

/* Byte 2 */
static volatile bool L_PRC_vbl_mirror_left				= false;
static volatile bool L_PRC_vbl_mirror_right				= false;
static volatile bool L_PRC_vbl_mirror_down				= false;
static volatile bool L_PRC_vbl_mirror_up				= false;
static volatile bool L_PRC_vbl_mirror_select			= false;	//left: false
static volatile bool L_PRC_vbl_mirror_heat				= false;
static volatile bool L_PRC_vbl_lock_button_pressed		= false;

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

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

/********  Initialisation  ***************************/
void PRC_v_init_f(void)
{
	PRC_stm_tx_message.id = 0x0120;
	PRC_stm_tx_message.rtr = 0;
	PRC_stm_tx_message.length = 3;
	PRC_stm_tx_message.data[0] = 0;
	PRC_stm_tx_message.data[1] = 0;	// Egy CAN üzenet összeállítása
	PRC_stm_tx_message.data[2] = 0;	// Egy CAN üzenet összeállítása
}

/********  Refresh controls  ***************************/
void PRC_v_refresh_control_f(void)
{
	L_PRC_vbl_RRD_lock_button_pressed = L_PRC_bl_get_msg_byte_bit_f(2,0);
}

/********  Process data  ***************************/
void PRC_v_process_f(void)
{
}

/********  Refresh status  ***************************/
void PRC_v_refresh_status_f(void)
{
	L_PRC_bl_set_msg_byte_bit_f(0,7,L_PRC_vbl_ECU_stat);
	L_PRC_bl_set_msg_byte_bit_f(0,6,L_PRC_vbl_window_motor_up_stat);
	L_PRC_bl_set_msg_byte_bit_f(0,5,L_PRC_vbl_window_motor_down_stat);
	L_PRC_bl_set_msg_byte_bit_f(0,0,L_PRC_vbl_lock_stat);

	L_PRC_bl_set_msg_byte_bit_f(1,7,L_PRC_vbl_right_rear_window_up);
	L_PRC_bl_set_msg_byte_bit_f(1,6,L_PRC_vbl_right_rear_window_down);
	L_PRC_bl_set_msg_byte_bit_f(1,5,L_PRC_vbl_left_rear_window_up);
	L_PRC_bl_set_msg_byte_bit_f(1,4,L_PRC_vbl_left_rear_window_down);
	L_PRC_bl_set_msg_byte_bit_f(1,3,L_PRC_vbl_right_front_window_up);
	L_PRC_bl_set_msg_byte_bit_f(1,2,L_PRC_vbl_right_front_window_down);
	L_PRC_bl_set_msg_byte_bit_f(1,1,L_PRC_vbl_left_front_window_up);
	L_PRC_bl_set_msg_byte_bit_f(1,0,L_PRC_vbl_left_front_window_down);

	L_PRC_bl_set_msg_byte_bit_f(2,7,L_PRC_vbl_mirror_left);
	L_PRC_bl_set_msg_byte_bit_f(2,6,L_PRC_vbl_mirror_right);
	L_PRC_bl_set_msg_byte_bit_f(2,5,L_PRC_vbl_mirror_down);
	L_PRC_bl_set_msg_byte_bit_f(2,4,L_PRC_vbl_mirror_up);
	L_PRC_bl_set_msg_byte_bit_f(2,3,L_PRC_vbl_mirror_select);
	L_PRC_bl_set_msg_byte_bit_f(2,2,L_PRC_vbl_mirror_heat);
	L_PRC_bl_set_msg_byte_bit_f(2,0,L_PRC_vbl_lock_button_pressed);
}


/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/

