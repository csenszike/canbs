
/*==============================================================[ INCLUDES ]=*/

#include "..\Common\platform.h"

#include "PRC.h"


/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

/* Left rear door control bits  (0x120)*/
// Byte 0
static volatile bool L_PRC_vbl_win_engine_up_control= false;
static volatile bool L_PRC_vbl_win_engine_down_control= false;
// Byte 1
static volatile bool L_PRC_vbl_win_mirror_left_control= false;
static volatile bool L_PRC_vbl_win_mirror_right_control= false;
static volatile bool L_PRC_vbl_win_mirror_up_control= false;
static volatile bool L_PRC_vbl_win_mirror_down_control= false;
static volatile bool L_PRC_vbl_win_mirror_select_control= false;
static volatile bool L_PRC_vbl_win_mirror_heat_control= false;
static volatile bool L_PRC_vbl_lock_leftbutton= false;

/* Left rear door status bits  (0x120)*/
// Byte 3
static volatile bool L_PRC_vbl_lock_leftstatus= false;
static volatile bool L_PRC_vbl_win_mirror_heat_leftstatus= false;


/* Own control bits (0x121)*/
// Byte 0
static volatile bool L_PRC_vbl_win_engine_up_rightbutton= false;
static volatile bool L_PRC_vbl_win_engine_down_rightbutton= false;
static volatile bool L_PRC_vbl_lock_rightbutton= false;

/* Own status bits (0x121)*/
// Byte 1
static volatile bool L_PRC_vbl_win_engine_up_status= false;
static volatile bool L_PRC_vbl_win_engine_down_status= false;
static volatile bool L_PRC_vbl_win_mirror_heat_rightstatus= false;
static volatile bool L_PRC_vbl_win_mirror_left_status= false;
static volatile bool L_PRC_vbl_win_mirror_right_status= false;
static volatile bool L_PRC_vbl_win_mirror_up_status= false;
static volatile bool L_PRC_vbl_win_mirror_down_status= false;
static volatile bool L_PRC_vbl_lock_rightstatus= false;
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


/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

/********  Initialisation  ***************************/
void PRC_v_init_f(void)
{
	PRC_stm_tx_message.id = 0x121;
	PRC_stm_tx_message.rtr = 0;
	PRC_stm_tx_message.length = 3;
	PRC_stm_tx_message.data[0] = 0;
	PRC_stm_tx_message.data[1] = 0;	
	PRC_stm_tx_message.data[2] = 0; // Egy CAN üzenet összeállítása
}

/********  Refresh controls  ***************************/
void PRC_v_refresh_control_f(void)
{
	L_PRC_vbl_win_engine_up_rightbutton=  DPY_TRM_S01__BUTTON_1_GET_STATE();
	L_PRC_vbl_win_engine_down_rightbutton=DPY_TRM_S01__BUTTON_2_GET_STATE();
	L_PRC_vbl_lock_rightbutton=           DPY_TRM_S01__BUTTON_3_GET_STATE();

	L_PRC_vbl_win_engine_up_control=    L_PRC_bl_get_msg_byte_bit_f(0,3);
	L_PRC_vbl_win_engine_down_control=  L_PRC_bl_get_msg_byte_bit_f(0,2);
	L_PRC_vbl_win_mirror_left_control=  L_PRC_bl_get_msg_byte_bit_f(1,7);
	L_PRC_vbl_win_mirror_right_control= L_PRC_bl_get_msg_byte_bit_f(1,6);
	L_PRC_vbl_win_mirror_down_control=  L_PRC_bl_get_msg_byte_bit_f(1,5);
	L_PRC_vbl_win_mirror_up_control=    L_PRC_bl_get_msg_byte_bit_f(1,4);
	L_PRC_vbl_win_mirror_select_control=L_PRC_bl_get_msg_byte_bit_f(1,3);
	L_PRC_vbl_win_mirror_heat_control=  L_PRC_bl_get_msg_byte_bit_f(1,2);
	L_PRC_vbl_lock_leftbutton=          L_PRC_bl_get_msg_byte_bit_f(1,0);
	
	L_PRC_vbl_lock_leftstatus= 			  L_PRC_bl_get_msg_byte_bit_f(4,7);
	L_PRC_vbl_win_mirror_heat_leftstatus= L_PRC_bl_get_msg_byte_bit_f(4,6);
}

/********  Process data  ***************************/
void PRC_v_process_f(void)
{
	/************** ABLAK ****************/
	// Ablak vezérlése a bal ajtóról  (amíg nyomva tartjuk a gombot, addig mozog az ablak)
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
	else if(L_PRC_vbl_win_engine_up_rightbutton || L_PRC_vbl_win_engine_down_rightbutton)
	{
		if(L_PRC_vbl_win_engine_up_rightbutton){        //Window Up 
			L_PRC_vbl_win_engine_down_status=false;	
			L_PRC_vbl_win_engine_up_status=true;
		}	 
		else if(L_PRC_vbl_win_engine_down_rightbutton){  //Window Down
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
	if(L_PRC_vbl_lock_leftbutton)
	{
		L_PRC_vbl_lock_rightstatus=~(L_PRC_vbl_lock_rightstatus);	
	}
	// Központi zár helyi vezérlése (ha megnyomjuk a gombot a központi zár állapota megváltozik)
	else if(L_PRC_vbl_lock_rightbutton)
	{
		L_PRC_vbl_lock_rightstatus=~(L_PRC_vbl_lock_rightstatus);
	}
	/************** TÜKÖR FÛTÉS ****************/	
	//	Tükörfûtés vezérlése a bal ajtóról  (ha megnyomjuk a gombot a fûtés állapota megváltozik)
	if(L_PRC_vbl_win_mirror_heat_control)
	{
		L_PRC_vbl_win_mirror_heat_rightstatus=~(L_PRC_vbl_win_mirror_heat_rightstatus); 
	}
	/************** TÜKÖR MOZGATÁS ****************/

	if(L_PRC_vbl_win_mirror_select_control==true)   // Ha a jobb oldali visszapillantó tükör van kiválasztva
	{
		if(L_PRC_vbl_win_mirror_left_control) L_PRC_vbl_win_mirror_left_status=true;
		else L_PRC_vbl_win_mirror_left_status=false;
		
		if(L_PRC_vbl_win_mirror_right_control) L_PRC_vbl_win_mirror_right_status=true;
		else L_PRC_vbl_win_mirror_right_status=false;

		if(L_PRC_vbl_win_mirror_down_control) L_PRC_vbl_win_mirror_down_status=true;
		else L_PRC_vbl_win_mirror_down_status=false;

		if(L_PRC_vbl_win_mirror_up_control) L_PRC_vbl_win_mirror_up_status=true;
		else L_PRC_vbl_win_mirror_up_status=false;
	}	

	/************** VEZÉRLÉS ****************/
	if(L_PRC_vbl_win_engine_up_status) DPY_TRM_S01__LED_1_ON();
	else DPY_TRM_S01__LED_1_OFF();

	if(L_PRC_vbl_win_engine_down_status) DPY_TRM_S01__LED_2_ON();
	else DPY_TRM_S01__LED_2_OFF();

	if(L_PRC_vbl_lock_rightstatus) DPY_TRM_S01__LED_3_ON();
	else DPY_TRM_S01__LED_3_OFF();

}

/********  Refresh status  ***************************/
void PRC_v_refresh_status_f(void)
{
	L_PRC_bl_set_msg_byte_bit_f(0,3,L_PRC_vbl_win_engine_up_rightbutton);
	L_PRC_bl_set_msg_byte_bit_f(0,2,L_PRC_vbl_win_engine_down_rightbutton);
	L_PRC_bl_set_msg_byte_bit_f(0,0,L_PRC_vbl_lock_rightbutton);
	L_PRC_bl_set_msg_byte_bit_f(1,7,L_PRC_vbl_win_engine_up_status);
	L_PRC_bl_set_msg_byte_bit_f(1,6,L_PRC_vbl_win_engine_down_status);
	L_PRC_bl_set_msg_byte_bit_f(1,5,L_PRC_vbl_win_mirror_heat_rightstatus);
	L_PRC_bl_set_msg_byte_bit_f(1,4,L_PRC_vbl_win_mirror_left_status);
	L_PRC_bl_set_msg_byte_bit_f(1,3,L_PRC_vbl_win_mirror_right_status);
	L_PRC_bl_set_msg_byte_bit_f(1,2,L_PRC_vbl_win_mirror_down_status);
	L_PRC_bl_set_msg_byte_bit_f(1,1,L_PRC_vbl_win_mirror_up_status);
	L_PRC_bl_set_msg_byte_bit_f(1,0,L_PRC_vbl_lock_rightstatus);
	L_PRC_bl_set_msg_byte_bit_f(0,7,L_PRC_vbl_ECU_status);
}



/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/

