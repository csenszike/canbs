
/*==============================================================[ INCLUDES ]=*/

#include "..\Common\platform.h"
#include "..\Common\mcp2515.h"
#include "..\Common\dpy_trm_s01.h"
#include "PRC.h"


/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

// variables for blinking indicator light
static volatile uint8_t L_PRC_vu8_counter=0;
static volatile bool L_PRC_vbl_blinker=false;

//variables for displaying lamps on 7-segment sisplay
static volatile uint8_t L_PRC_dpy_data1=1;
static volatile uint8_t L_PRC_dpy_data2=1;


// variables for detecting rising edge on wheel buttons (memory for the last state)
static volatile bool L_PRC_vbl_sidelight_button_prev=false;
static volatile bool L_PRC_vbl_lowbeam_button_prev=false;
static volatile bool L_PRC_vbl_highbeam_button_prev=false;
static volatile bool L_PRC_vbl_headfog_button_prev=false;
//static volatile bool L_PRC_vbl_tailfog_button_prev=false;
static volatile bool L_PRC_vbl_leftind_button_prev=false;
static volatile bool L_PRC_vbl_rightind_button_prev=false;

// State variables of lights (except of brake light)
static volatile bool L_PRC_vbl_sidelight_state=false;
static volatile bool L_PRC_vbl_lowbeam_state=false;
static volatile bool L_PRC_vbl_highbeam_state=false;
static volatile bool L_PRC_vbl_headfog_state=false;
static volatile bool L_PRC_vbl_tailfog_state=false;
static volatile bool L_PRC_vbl_leftind_state=false;
static volatile bool L_PRC_vbl_rightind_state=false;

// Buttons from wheel

/* Control byte from brake pedal (0x100) */
// Byte 3
//static volatile uint8_t L_PRC_vu8_brake_pedal=0x00;
/* Control bits from wheel (0x100) */
// Byte 4
// Bit 3 - sidelight
static volatile bool L_PRC_vbl_sidelight_button=false;
// Bit 2 - low beam
static volatile bool L_PRC_vbl_lowbeam_button=false;
// Bit 1 - high beam
static volatile bool L_PRC_vbl_highbeam_button=false;
// Bit 5 - front fog light
static volatile bool L_PRC_vbl_headfog_button=false;
// Bit 4 - rear fog light
static volatile bool L_PRC_vbl_tailfog_button=false;
// Bit 7 - left indicator
static volatile bool L_PRC_vbl_leftind_button=false;
// Bit 6 - rigt indicator
static volatile bool L_PRC_vbl_rightind_button=false;

/* Own status bits  (0x110) */
// Byte 0
static volatile bool L_PRC_vbl_ecu_status=true;
static volatile bool L_PRC_vbl_highbeam_status=false;
static volatile bool L_PRC_vbl_lowbeam_status=false;
static volatile bool L_PRC_vbl_headfog_status=false;
static volatile bool L_PRC_vbl_rightind_status=false;
static volatile bool L_PRC_vbl_leftind_status=false;
static volatile bool L_PRC_vbl_sidelight_status=false;

/* Control bits for tail lights (0x110) */
// Byte 1
static volatile bool L_PRC_vbl_sidelight_command=false;
static volatile bool L_PRC_vbl_leftind_command=false;
static volatile bool L_PRC_vbl_rightind_command=false;



/*======================================================[ EXTERNAL GLOBALS ]=*/

CAN_st_message_t PRC_stm_tx_message;

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

static bool L_PRC_bl_get_msg_byte_bit_f(uint8_t byte,uint8_t bit);
static void L_PRC_v_set_msg_byte_bit_f(uint8_t byte,uint8_t bit,bool value);

// Handling lights
static void L_PRC_v_sidelight_on_f(void);
static void L_PRC_v_sidelight_off_f(void);
static bool L_PRC_v_sidelight_getstatus_f(void);
static void L_PRC_v_lowbeam_on_f(void);
static void L_PRC_v_lowbeam_off_f(void);
static bool L_PRC_v_lowbeam_getstatus_f(void);
static void L_PRC_v_highbeam_on_f(void);
static void L_PRC_v_highbeam_off_f(void);
static bool L_PRC_v_highbeam_getstatus_f(void);
static void L_PRC_v_headfog_on_f(void);
static void L_PRC_v_headfog_off_f(void);
static bool L_PRC_v_headfog_getstatus_f(void);
static void L_PRC_v_leftind_on_f(void);
static void L_PRC_v_leftind_off_f(void);
static bool L_PRC_v_leftind_getstatus_f(void);
static void L_PRC_v_rightind_on_f(void);
static void L_PRC_v_rightind_off_f(void);
static bool L_PRC_v_rightind_getstatus_f(void);

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

static bool L_PRC_bl_get_msg_byte_bit_f(uint8_t byte,uint8_t bit)
{
	return ((CAN_msg_rx_message.data[byte]>>bit)&0x01);
}

static void L_PRC_v_set_msg_byte_bit_f(uint8_t byte,uint8_t bit,bool value)
{
	if(value)
		PRC_stm_tx_message.data[byte] |= (1<<bit);
	else
		PRC_stm_tx_message.data[byte] &= ~(1<<bit);

}

// Handling bulbs
static void L_PRC_v_sidelight_on_f(void)
{
	DPY_TRM_S01__LED_4_ON();
}
static void L_PRC_v_sidelight_off_f(void)
{
	DPY_TRM_S01__LED_4_OFF();
}
static bool L_PRC_v_sidelight_getstatus_f(void)
{
	return DPY_TRM_S01__LED_4_GET_STATE();
}
static void L_PRC_v_lowbeam_on_f(void)
{
	L_PRC_dpy_data1=8;
	DPY_u8_trm_s01__7seq_write_number_f(L_PRC_dpy_data1*10+L_PRC_dpy_data2,0);

}
static void L_PRC_v_lowbeam_off_f(void)
{
	L_PRC_dpy_data1=1;
	DPY_u8_trm_s01__7seq_write_number_f(L_PRC_dpy_data1*10+L_PRC_dpy_data2,0);
}
static bool L_PRC_v_lowbeam_getstatus_f(void)
{
	if (L_PRC_dpy_data1==8)
		return true;
	else
		return false;

}
static void L_PRC_v_highbeam_on_f(void)
{
	L_PRC_dpy_data2=8;
	DPY_u8_trm_s01__7seq_write_number_f(L_PRC_dpy_data1*10+L_PRC_dpy_data2,0);
}
static void L_PRC_v_highbeam_off_f(void)
{
	L_PRC_dpy_data2=1;
	DPY_u8_trm_s01__7seq_write_number_f(L_PRC_dpy_data1*10+L_PRC_dpy_data2,0);
}
static bool L_PRC_v_highbeam_getstatus_f(void)
{
	if (L_PRC_dpy_data2==8)
		return true;
	else
		return false;
}
static void L_PRC_v_headfog_on_f(void)
{
	DPY_TRM_S01__LED_3_ON();
}
static void L_PRC_v_headfog_off_f(void)
{
	DPY_TRM_S01__LED_3_OFF();
}
static bool L_PRC_v_headfog_getstatus_f(void)
{
	return DPY_TRM_S01__LED_3_GET_STATE();
}
static void L_PRC_v_leftind_on_f(void)
{
	DPY_TRM_S01__LED_1_ON();
}
static void L_PRC_v_leftind_off_f(void)
{
	DPY_TRM_S01__LED_1_OFF();
}
static bool L_PRC_v_leftind_getstatus_f(void)
{
	return DPY_TRM_S01__LED_1_GET_STATE();
}
static void L_PRC_v_rightind_on_f(void)
{
	DPY_TRM_S01__LED_2_ON();
}
static void L_PRC_v_rightind_off_f(void)
{
	DPY_TRM_S01__LED_2_OFF();
}

static bool L_PRC_v_rightind_getstatus_f(void)
{
	return DPY_TRM_S01__LED_2_GET_STATE();
}

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

void PRC_v_init_f(void)
{
	// Constructing an empty message frame
	PRC_stm_tx_message.id = 0x0110;
	PRC_stm_tx_message.rtr = 0;
	PRC_stm_tx_message.length = 2;
	PRC_stm_tx_message.data[0] = 1;
	PRC_stm_tx_message.data[1] = 1;	// Egy CAN üzenet összeállítása
	
	// Initializing display
	DPY_v_trm_s01__7seq_clear_dpy_f();
	DPY_u8_trm_s01__7seq_write_number_f(L_PRC_dpy_data1*10+L_PRC_dpy_data2,0);
	

}

void PRC_v_refresh_remote_control_f(void)
{
	if (CAN_msg_rx_message.id==0x100)
	{
		/* Control byte from brake pedal (0x100) */
		// Byte 3
		//L_PRC_vu8_brake_pedal=CAN_msg_rx_message.data[3];
		/* Control bits from wheel (0x100) */
		// Byte 4
		// Bit 3 - sidelight
		L_PRC_vbl_sidelight_button=L_PRC_bl_get_msg_byte_bit_f(4,3);
		// Bit 2 - low beam
		L_PRC_vbl_lowbeam_button=L_PRC_bl_get_msg_byte_bit_f(4,2);
		// Bit 1 - high beam
		L_PRC_vbl_highbeam_button=L_PRC_bl_get_msg_byte_bit_f(4,1);
		// Bit 5 - front fog light
		L_PRC_vbl_headfog_button=L_PRC_bl_get_msg_byte_bit_f(4,5);
		// Bit 4 - rear fog light
		L_PRC_vbl_tailfog_button=L_PRC_bl_get_msg_byte_bit_f(4,4);
		// Bit 7 - left indicator
		L_PRC_vbl_leftind_button=L_PRC_bl_get_msg_byte_bit_f(4,7);
		// Bit 6 - rigt indicator
		L_PRC_vbl_rightind_button=L_PRC_bl_get_msg_byte_bit_f(4,6);
	}

}
void PRC_v_process_f(void)
{
//1./////////////////////////////
	// sidelight //
	// Detecting rising edge
	if ((L_PRC_vbl_sidelight_button_prev==0)&&(L_PRC_vbl_sidelight_button==1))
	{
		//Changing state of the lamp
		L_PRC_vbl_sidelight_state=!L_PRC_vbl_sidelight_state;
		// Turning on or off the lamp depending on the state
		if (L_PRC_vbl_sidelight_state)
		{
			L_PRC_v_sidelight_on_f();
			L_PRC_vbl_sidelight_command=true;
		}
		else
		{
			L_PRC_v_sidelight_off_f();
			L_PRC_vbl_sidelight_command=false;
		}
	}
	//Shifting button states
	L_PRC_vbl_sidelight_button_prev=L_PRC_vbl_sidelight_button;

//2./////////////////////////
	// left index
	// Detecting rising edge
	if ((L_PRC_vbl_leftind_button_prev==0)&&(L_PRC_vbl_leftind_button==1))
	{
		//Changing state of the lamp
		L_PRC_vbl_leftind_state=!L_PRC_vbl_leftind_state;
		// Turning on or off the lamp depending on the state
		if (L_PRC_vbl_leftind_state)
		{
			//functionality is at the timer IT subroutine
		}
		else
		{
			//functionality is at the timer IT subroutine
		}
	}
	//Shifting button states
	L_PRC_vbl_leftind_button_prev=L_PRC_vbl_leftind_button;

//3./////////////////////////
	// right index
	// Detecting rising edge
	if ((L_PRC_vbl_rightind_button_prev==0)&&(L_PRC_vbl_rightind_button==1))
	{
		//Changing state of the lamp
		L_PRC_vbl_rightind_state=!L_PRC_vbl_rightind_state;
		// Turning on or off the lamp depending on the state
		if (L_PRC_vbl_rightind_state)
		{
			//functionality is at the timer IT subroutine
		}
		else
		{
			//functionality is at the timer IT subroutine
		}
	}
	//Shifting button states
	L_PRC_vbl_rightind_button_prev=L_PRC_vbl_rightind_button;

//4.//////////////////////
	// Low beam
	// Detecting rising edge
	if ((L_PRC_vbl_lowbeam_button_prev==0)&&(L_PRC_vbl_lowbeam_button==1))
	{
		//Changing state of the lamp
		L_PRC_vbl_lowbeam_state=!L_PRC_vbl_lowbeam_state;
		// Turning on or off the lamp depending on the state
		if (L_PRC_vbl_lowbeam_state)
		{
			L_PRC_v_lowbeam_on_f();
		}
		else
		{
			L_PRC_v_lowbeam_off_f();
		}
	}
	//Shifting button states
	L_PRC_vbl_lowbeam_button_prev=L_PRC_vbl_lowbeam_button;

//5.//////////////////////
	// High beam
	// Detecting rising edge
	if ((L_PRC_vbl_highbeam_button_prev==0)&&(L_PRC_vbl_highbeam_button==1))
	{
		//Changing state of the lamp
		L_PRC_vbl_highbeam_state=!L_PRC_vbl_highbeam_state;
		// Turning on or off the lamp depending on the state
		if (L_PRC_vbl_highbeam_state)
		{
			L_PRC_v_highbeam_on_f();
		}
		else
		{
			L_PRC_v_highbeam_off_f();
		}
	}
	//Shifting button states
	L_PRC_vbl_highbeam_button_prev=L_PRC_vbl_highbeam_button;

//6.//////////////////////
	// Head fog light
	// Detecting rising edge
	if ((L_PRC_vbl_headfog_button_prev==0)&&(L_PRC_vbl_headfog_button==1))
	{
		//Changing state of the lamp
		L_PRC_vbl_headfog_state=!L_PRC_vbl_headfog_state;
		// Turning on or off the lamp depending on the state
		if (L_PRC_vbl_headfog_state)
		{
			L_PRC_v_headfog_on_f();
		}
		else
		{
			L_PRC_v_headfog_off_f();
		}
	}
	//Shifting button states
	L_PRC_vbl_headfog_button_prev=L_PRC_vbl_headfog_button;



}

void PRC_v_refresh_local_control_f(void)
{
	//There is no local control
}

void PRC_v_refresh_local_status_f(void)
{
	// Getting real state of the lights (bulb is OK or not)
	L_PRC_vbl_sidelight_status=L_PRC_v_sidelight_getstatus_f();
	L_PRC_vbl_lowbeam_status=L_PRC_v_lowbeam_getstatus_f();
	L_PRC_vbl_highbeam_status=L_PRC_v_highbeam_getstatus_f();
	L_PRC_vbl_headfog_status=L_PRC_v_headfog_getstatus_f();
	L_PRC_vbl_leftind_status=L_PRC_v_leftind_getstatus_f();
	L_PRC_vbl_rightind_status=L_PRC_v_rightind_getstatus_f();
}


void PRC_v_refresh_message_status_f(void)
{
	L_PRC_v_set_msg_byte_bit_f(0,7,L_PRC_vbl_ecu_status);
	L_PRC_v_set_msg_byte_bit_f(0,5,L_PRC_vbl_highbeam_status);
	L_PRC_v_set_msg_byte_bit_f(0,4,L_PRC_vbl_lowbeam_status);
	L_PRC_v_set_msg_byte_bit_f(0,3,L_PRC_vbl_headfog_status);
	L_PRC_v_set_msg_byte_bit_f(0,2,L_PRC_vbl_rightind_status);
	L_PRC_v_set_msg_byte_bit_f(0,1,L_PRC_vbl_leftind_status);
	L_PRC_v_set_msg_byte_bit_f(0,0,L_PRC_vbl_sidelight_status);
	//Refreshing command
	L_PRC_v_set_msg_byte_bit_f(1,0,L_PRC_vbl_rightind_command);
	L_PRC_v_set_msg_byte_bit_f(1,1,L_PRC_vbl_leftind_command);
	L_PRC_v_set_msg_byte_bit_f(1,2,L_PRC_vbl_sidelight_command);

}
void PRC_v_20mstick_f(void)
{
	L_PRC_vu8_counter+=1;
	if (L_PRC_vu8_counter==24)		//0.5 sec. tick
	{
		L_PRC_vbl_blinker=!L_PRC_vbl_blinker;
		L_PRC_vu8_counter=0;

		//Left index
		if (L_PRC_vbl_leftind_state)
		{
			//If left indicator is turned on
			if (L_PRC_vbl_blinker)
			{
				L_PRC_v_leftind_on_f();
				L_PRC_vbl_leftind_command=true;

			}
			else
			{
				L_PRC_v_leftind_off_f();
				L_PRC_vbl_leftind_command=false;

			}
		}
		else
		{
			L_PRC_v_leftind_off_f();
			L_PRC_vbl_leftind_command=false;
		}

		//Right index
		if (L_PRC_vbl_rightind_state)
		{
			//If left indicator is turned on
			if (L_PRC_vbl_blinker)
			{
				L_PRC_v_rightind_on_f();
				L_PRC_vbl_rightind_command=true;

			}
			else
			{
				L_PRC_v_rightind_off_f();
				L_PRC_vbl_rightind_command=false;

			}
		}
		else
		{
			L_PRC_v_rightind_off_f();
			L_PRC_vbl_rightind_command=false;
		}


		
	}
	
}

/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/

