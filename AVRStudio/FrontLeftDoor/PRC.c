/**@file PRC.c
* @brief Front Left Door process program
* @author Csanad Erdos
* @version 1.1
* @date 2010-11-11
*/
/*****************************************************************************
* Title: Rendszertervezés HF                                                 *
* Hardware: CAN/LIN extension board for mitmót system                        *
* Processor: ATMEGA128                                                       *
* Author: Csanad Erdos                                                       *
* Date:               					                                     *
* Compiler: avr-gcc                                                          *
* ------------------------------                                             *
* Description: Front Left Door process									     *
******************************************************************************/


/*==============================================================[ INCLUDES ]=*/

#include "..\Common\platform.h"

#include "..\Common\dpy_trm_s01.h"
#include "..\Common\mcp2515.h"

#include "PRC.h"


/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

const unsigned char s_n[13]={
	0x28, 0xF9, 0x1C,
	0x58, 0xC9, 0x4A,
	0x0A, 0xF8, 0x08,
	0x48, 0xFF, 0xDF, 0xF7};

#define SEG_SPACE 10
#define SEG_MIN	  11
#define SEG_DOT	  12

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

/* Right rear door control bits */
/* Byte 0 */
static volatile bool L_PRC_vbl_RRD_lock_button_pressed		= false;	//0

/* Own 'kezelõszerv' states */
/* Byte 0 */
static volatile bool L_PRC_vbl_right_rear_window_up_bs		= false;	//7
static volatile bool L_PRC_vbl_right_rear_window_down_bs	= false;	//6
static volatile bool L_PRC_vbl_left_rear_window_up_bs		= false;	//5
static volatile bool L_PRC_vbl_left_rear_window_down_bs		= false;	//4
static volatile bool L_PRC_vbl_right_front_window_up_bs		= false;	//3
static volatile bool L_PRC_vbl_right_front_window_down_bs	= false;	//2
static volatile bool L_PRC_vbl_left_front_window_up_bs		= false;	//1
static volatile bool L_PRC_vbl_left_front_window_down_bs	= false;	//0

/* Byte 1 */
static volatile bool L_PRC_vbl_mirror_left_bs				= false;	//7
static volatile bool L_PRC_vbl_mirror_right_bs				= false;	//6
static volatile bool L_PRC_vbl_mirror_down_bs				= false;	//5
static volatile bool L_PRC_vbl_mirror_up_bs					= false;	//5
static volatile bool L_PRC_vbl_mirror_select_bs				= false;	//3	//left: false
static volatile bool L_PRC_vbl_mirror_heat_bs				= false;	//2
static volatile bool L_PRC_vbl_lock_button_pressed_bs		= false;	//0

/* Own 'beavatkozó szerv' states */
/* Byte 2 */
static volatile bool L_PRC_vbl_window_motor_up_es			= false;	//7
static volatile bool L_PRC_vbl_window_motor_down_es			= false;	//6
static volatile bool L_PRC_vbl_mirror_heat_es				= false;	//5
static volatile bool L_PRC_vbl_mirror_left_es				= false;	//4
static volatile bool L_PRC_vbl_mirror_right_es				= false;	//3
static volatile bool L_PRC_vbl_mirror_down_es				= false;	//2
static volatile bool L_PRC_vbl_mirror_up_es					= false;	//1
static volatile bool L_PRC_vbl_lock_es						= false;	//0

/* Byte 3 */
static volatile bool L_PRC_vbl_ECU_es						= false;	//7

/* Byte 4 */
static volatile bool L_PRC_vbl_lock_control					= false;	//7
static volatile bool L_PRC_vbl_mirror_heat_control			= false;	//6


/* For state machines */

static volatile bool L_PRC_vbl_lock_or		= false;
static volatile bool L_PRC_vbl_lock_prev	= false;
static volatile bool L_PRC_vbl_heat_prev	= false;

//variables for displaying lamps on 7-segment sisplay
static volatile uint8_t L_PRC_dpy_data1=SEG_SPACE;
static volatile uint8_t L_PRC_dpy_data2=SEG_SPACE;
static volatile uint8_t L_PRC_dpy_data3=SEG_SPACE;

/*======================================================[ EXTERNAL GLOBALS ]=*/

CAN_st_message_t PRC_stm_tx_message;

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

static bool L_PRC_bl_get_msg_byte_bit_f(uint8_t byte,uint8_t bit);
static void L_PRC_bl_set_msg_byte_bit_f(uint8_t byte,uint8_t bit,bool value);

static void L_PRC_v_lock_on_f(void);
static void L_PRC_v_lock_off_f(void);
static bool L_PRC_v_lock_getstatus_f(void);
static void L_PRC_v_heat_on_f(void);
static void L_PRC_v_heat_off_f(void);
static bool L_PRC_v_heat_getstatus_f(void);
static void L_PRC_v_window_up_f(void);
static void L_PRC_v_window_down_f(void);
static void L_PRC_v_window_stop_f(void);
static bool L_PRC_v_window_up_getstatus_f(void);
static bool L_PRC_v_window_down_getstatus_f(void);
static void L_PRC_v_mirror_stop_f(void);
static void L_PRC_v_mirror_left_f(void);
static void L_PRC_v_mirror_right_f(void);
static void L_PRC_v_mirror_down_f(void);
static void L_PRC_v_mirror_up_f(void);
static bool L_PRC_v_mirror_left_getstatus_f(void);
static bool L_PRC_v_mirror_right_getstatus_f(void);
static bool L_PRC_v_mirror_down_getstatus_f(void);
static bool L_PRC_v_mirror_up_getstatus_f(void);

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

/********* KÖZPONTI ZÁR ***********/
static void L_PRC_v_lock_on_f(void)
{
	DPY_TRM_S01__LED_1_ON();
}
static void L_PRC_v_lock_off_f(void)
{
	DPY_TRM_S01__LED_1_OFF();
}
static bool L_PRC_v_lock_getstatus_f(void)
{
	return DPY_TRM_S01__LED_1_GET_STATE();
}

/********* Tükör FÛTÉS ***********/
static void L_PRC_v_heat_on_f(void)
{
	DPY_TRM_S01__LED_2_ON();
}
static void L_PRC_v_heat_off_f(void)
{
	DPY_TRM_S01__LED_2_OFF();
}
static bool L_PRC_v_heat_getstatus_f(void)
{
	return DPY_TRM_S01__LED_2_GET_STATE();
}

/*********** ABLAK ***************/

static void L_PRC_v_window_up_f(void)
{
	L_PRC_dpy_data1=1;
	dpy_trm_s01__7seq_write_3digit(s_n[L_PRC_dpy_data3],s_n[L_PRC_dpy_data2],s_n[L_PRC_dpy_data1]);
}

static void L_PRC_v_window_down_f(void)
{
	L_PRC_dpy_data1=2;
	dpy_trm_s01__7seq_write_3digit(s_n[L_PRC_dpy_data3],s_n[L_PRC_dpy_data2],s_n[L_PRC_dpy_data1]);
}

static void L_PRC_v_window_stop_f(void)
{
	L_PRC_dpy_data1=0;
	dpy_trm_s01__7seq_write_3digit(s_n[L_PRC_dpy_data3],s_n[L_PRC_dpy_data2],s_n[L_PRC_dpy_data1]);
}

static bool L_PRC_v_window_up_getstatus_f(void)
{
	if (L_PRC_dpy_data1==1)
		return true;
	else
		return false;
}

static bool L_PRC_v_window_down_getstatus_f(void)
{
	if (L_PRC_dpy_data1==2)
		return true;
	else
		return false;
}

/*********** Tükör mozgatás ***************/

static void L_PRC_v_mirror_stop_f(void)
{
	L_PRC_dpy_data2=0;
	dpy_trm_s01__7seq_write_3digit(s_n[L_PRC_dpy_data3],s_n[L_PRC_dpy_data2],s_n[L_PRC_dpy_data1]);
}

static void L_PRC_v_mirror_left_f(void)
{
	L_PRC_dpy_data2=4;
	dpy_trm_s01__7seq_write_3digit(s_n[L_PRC_dpy_data3],s_n[L_PRC_dpy_data2],s_n[L_PRC_dpy_data1]);
}

static void L_PRC_v_mirror_right_f(void)
{
	L_PRC_dpy_data2=3;
	dpy_trm_s01__7seq_write_3digit(s_n[L_PRC_dpy_data3],s_n[L_PRC_dpy_data2],s_n[L_PRC_dpy_data1]);
}

static void L_PRC_v_mirror_down_f(void)
{
	L_PRC_dpy_data2=2;
	dpy_trm_s01__7seq_write_3digit(s_n[L_PRC_dpy_data3],s_n[L_PRC_dpy_data2],s_n[L_PRC_dpy_data1]);
}

static void L_PRC_v_mirror_up_f(void)
{
	L_PRC_dpy_data2=1;
	dpy_trm_s01__7seq_write_3digit(s_n[L_PRC_dpy_data3],s_n[L_PRC_dpy_data2],s_n[L_PRC_dpy_data1]);
}

static bool L_PRC_v_mirror_left_getstatus_f(void)
{
	if (L_PRC_dpy_data2==4)
		return true;
	else
		return false;
}

static bool L_PRC_v_mirror_right_getstatus_f(void)
{
	if (L_PRC_dpy_data2==3)
		return true;
	else
		return false;
}

static bool L_PRC_v_mirror_down_getstatus_f(void)
{
	if (L_PRC_dpy_data2==2)
		return true;
	else
		return false;
}

static bool L_PRC_v_mirror_up_getstatus_f(void)
{
	if (L_PRC_dpy_data2==1)
		return true;
	else
		return false;
}

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

/********  Initialisation  ***************************/
void PRC_v_init_f(void)
{
	PRC_stm_tx_message.id = 0x0120;
	PRC_stm_tx_message.rtr = 0;
	PRC_stm_tx_message.length = 5;
	PRC_stm_tx_message.data[0] = 0;
	PRC_stm_tx_message.data[1] = 0;
	PRC_stm_tx_message.data[2] = 0;
	PRC_stm_tx_message.data[3] = 0;
	PRC_stm_tx_message.data[4] = 0;
}

/********  Refresh controls  ***************************/
void PRC_v_refresh_remote_control_f(void)
{
	/********* LOCK ***********/
	/* Refresh lock button states */
	L_PRC_vbl_RRD_lock_button_pressed = L_PRC_bl_get_msg_byte_bit_f(0,0);
}

/********  Refresh msg status  ***************************/
void PRC_v_refresh_message_status_f(void)
{
	/* Byte 0 */
	L_PRC_bl_set_msg_byte_bit_f(0,7,L_PRC_vbl_right_rear_window_up_bs);
	L_PRC_bl_set_msg_byte_bit_f(0,6,L_PRC_vbl_right_rear_window_down_bs);
	L_PRC_bl_set_msg_byte_bit_f(0,5,L_PRC_vbl_left_rear_window_up_bs);
	L_PRC_bl_set_msg_byte_bit_f(0,4,L_PRC_vbl_left_rear_window_down_bs);
	L_PRC_bl_set_msg_byte_bit_f(0,3,L_PRC_vbl_right_front_window_up_bs);
	L_PRC_bl_set_msg_byte_bit_f(0,2,L_PRC_vbl_right_front_window_down_bs);
	L_PRC_bl_set_msg_byte_bit_f(0,1,L_PRC_vbl_left_front_window_up_bs);
	L_PRC_bl_set_msg_byte_bit_f(0,0,L_PRC_vbl_left_front_window_down_bs);

	/* Byte 1 */
	L_PRC_bl_set_msg_byte_bit_f(1,7,L_PRC_vbl_mirror_left_bs);
	L_PRC_bl_set_msg_byte_bit_f(1,6,L_PRC_vbl_mirror_right_bs);
	L_PRC_bl_set_msg_byte_bit_f(1,5,L_PRC_vbl_mirror_down_bs);
	L_PRC_bl_set_msg_byte_bit_f(1,4,L_PRC_vbl_mirror_up_bs);
	L_PRC_bl_set_msg_byte_bit_f(1,3,L_PRC_vbl_mirror_select_bs);
	L_PRC_bl_set_msg_byte_bit_f(1,2,L_PRC_vbl_mirror_heat_bs);
	L_PRC_bl_set_msg_byte_bit_f(1,0,L_PRC_vbl_lock_button_pressed_bs);

	/* Byte 2 */
	L_PRC_bl_set_msg_byte_bit_f(2,7,L_PRC_vbl_window_motor_up_es);
	L_PRC_bl_set_msg_byte_bit_f(2,6,L_PRC_vbl_window_motor_down_es);
	L_PRC_bl_set_msg_byte_bit_f(2,5,L_PRC_vbl_mirror_heat_es);
	L_PRC_bl_set_msg_byte_bit_f(2,4,L_PRC_vbl_mirror_left_es);
	L_PRC_bl_set_msg_byte_bit_f(2,3,L_PRC_vbl_mirror_right_es);
	L_PRC_bl_set_msg_byte_bit_f(2,2,L_PRC_vbl_mirror_down_es);
	L_PRC_bl_set_msg_byte_bit_f(2,2,L_PRC_vbl_mirror_up_es);
	L_PRC_bl_set_msg_byte_bit_f(2,0,L_PRC_vbl_lock_es);

	/* Byte 3 */
	L_PRC_bl_set_msg_byte_bit_f(3,7,L_PRC_vbl_ECU_es);

	/* Byte 4 */
	L_PRC_bl_set_msg_byte_bit_f(4,7,L_PRC_vbl_lock_control);
	L_PRC_bl_set_msg_byte_bit_f(4,6,L_PRC_vbl_mirror_heat_control);
}

/********  Refresh local controls  ***************************/
void PRC_v_refresh_local_control_f()
{
	/* mirror select switch status */
	L_PRC_vbl_mirror_select_bs = DPY_TRM_S01__SWITCH_1_GET_STATE();

	/* SW4 = 1 => lock, window up/down */
	if(DPY_TRM_S01__SWITCH_4_GET_STATE())
	{
		/* heat button status */
		L_PRC_vbl_mirror_heat_bs = !DPY_TRM_S01__BUTTON_1_GET_STATE();

		/* mirror button status */
		L_PRC_vbl_mirror_up_bs = !DPY_TRM_S01__BUTTON_2_GET_STATE();
		L_PRC_vbl_mirror_down_bs = !DPY_TRM_S01__BUTTON_3_GET_STATE();
	}
	/* SW4 = 0 => heat, mirror up/down */
	else
	{
		/* lock button status */
		L_PRC_vbl_lock_button_pressed_bs = !DPY_TRM_S01__BUTTON_1_GET_STATE();

		/* SW2:3 = 00 */
		if(!DPY_TRM_S01__SWITCH_2_GET_STATE() && !DPY_TRM_S01__SWITCH_3_GET_STATE())
		{
			/* left front window button status */
			L_PRC_vbl_left_front_window_up_bs = !DPY_TRM_S01__BUTTON_2_GET_STATE();
			L_PRC_vbl_left_front_window_down_bs = !DPY_TRM_S01__BUTTON_3_GET_STATE();
		}
		/* SW2:3 = 01 */
		else if(!DPY_TRM_S01__SWITCH_2_GET_STATE() && DPY_TRM_S01__SWITCH_3_GET_STATE())
		{
			/* right front window button status */
			L_PRC_vbl_right_front_window_up_bs = !DPY_TRM_S01__BUTTON_2_GET_STATE();
			L_PRC_vbl_right_front_window_down_bs = !DPY_TRM_S01__BUTTON_3_GET_STATE();
		}
		/* SW2:3 = 10 */
		else if(DPY_TRM_S01__SWITCH_2_GET_STATE() && !DPY_TRM_S01__SWITCH_3_GET_STATE())
		{
			/* left rear window button status */
			L_PRC_vbl_left_rear_window_up_bs = !DPY_TRM_S01__BUTTON_2_GET_STATE();
			L_PRC_vbl_left_rear_window_down_bs = !DPY_TRM_S01__BUTTON_3_GET_STATE();
		}
		/* SW2:3 = 11 */
		else if(DPY_TRM_S01__SWITCH_2_GET_STATE() && DPY_TRM_S01__SWITCH_3_GET_STATE())
		{
			/* right rear window button status */
			L_PRC_vbl_right_rear_window_up_bs = !DPY_TRM_S01__BUTTON_2_GET_STATE();
			L_PRC_vbl_right_rear_window_down_bs = !DPY_TRM_S01__BUTTON_3_GET_STATE();
		}
	}

}

/********  Process data  ***************************/
void PRC_v_process_f(void)
{
	L_PRC_vbl_lock_or = L_PRC_vbl_RRD_lock_button_pressed || L_PRC_vbl_lock_button_pressed_bs;

	/* Changing lock state on rising edge */
	if(L_PRC_vbl_lock_or && !L_PRC_vbl_lock_prev)
		L_PRC_vbl_lock_control = !L_PRC_vbl_lock_control;

	L_PRC_vbl_lock_prev = L_PRC_vbl_lock_or;

	/* Changing heat state on rising edge */
	if(L_PRC_vbl_mirror_heat_bs && !L_PRC_vbl_heat_prev)
		L_PRC_vbl_mirror_heat_control = !L_PRC_vbl_mirror_heat_control;

	L_PRC_vbl_heat_prev = L_PRC_vbl_mirror_heat_bs;

	/************** ABLAK ****************/
	// Ablak helyi vezérlése (amíg nyomva tartjuk a gombot, addig mozog az ablak)
	if(L_PRC_vbl_left_front_window_up_bs || L_PRC_vbl_left_front_window_down_bs)
	{
		if(L_PRC_vbl_left_front_window_up_bs)        //Window Up 
		{
			L_PRC_v_window_up_f();
		}	 
		else if(L_PRC_vbl_left_front_window_down_bs)  //Window Down
		{
			L_PRC_v_window_down_f();
		}  
	}
	else
	{
		L_PRC_v_window_stop_f();
	}

	/************** TÜKÖR ****************/
	// Tükör helyi vezérlése (amíg nyomva tartjuk a gombot, addig mozog a tükör)
	// Ha a jobb oldali visszapillantó tükör van kiválasztva
	if(!L_PRC_vbl_mirror_select_bs)   
	{
		if(L_PRC_vbl_mirror_left_bs) 		L_PRC_v_mirror_left_f();
		else if(L_PRC_vbl_mirror_right_bs)	L_PRC_v_mirror_right_f();
		else if(L_PRC_vbl_mirror_down_bs)	L_PRC_v_mirror_down_f();
		else if(L_PRC_vbl_mirror_up_bs)		L_PRC_v_mirror_up_f();
		else								L_PRC_v_mirror_stop_f();		
	}
	else	L_PRC_v_mirror_stop_f();

	/************** KÖZPONTI ZÁR ****************/
	if(L_PRC_vbl_lock_control)				L_PRC_v_lock_on_f();
	else									L_PRC_v_lock_off_f();	

	/************** TÜKÖR FÛTÉS ****************/
	if(L_PRC_vbl_mirror_heat_control)		L_PRC_v_heat_on_f();
	else									L_PRC_v_heat_off_f();

}

/********  Refresh local status  ***************************/
void PRC_v_refresh_local_status_f()
{
	// Getting real state of the actuators
	L_PRC_vbl_window_motor_down_es	= L_PRC_v_window_down_getstatus_f();
	L_PRC_vbl_window_motor_up_es	= L_PRC_v_window_up_getstatus_f();
	L_PRC_vbl_lock_es				= L_PRC_v_lock_getstatus_f();
	L_PRC_vbl_mirror_heat_es		= L_PRC_v_heat_getstatus_f();
	L_PRC_vbl_mirror_left_es		= L_PRC_v_mirror_left_getstatus_f();
	L_PRC_vbl_mirror_right_es		= L_PRC_v_mirror_right_getstatus_f();
	L_PRC_vbl_mirror_down_es		= L_PRC_v_mirror_down_getstatus_f();
	L_PRC_vbl_mirror_up_es			= L_PRC_v_mirror_up_getstatus_f();

}

/********  PRC 20mstick  ***************************/
void PRC_v_20mstick_f()
{
}

/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/

