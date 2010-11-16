
/*==============================================================[ INCLUDES ]=*/

#include "..\Common\platform.h"

#include "PRC.h"


/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

#define SEG_SPACE 10

/* Number Table 0 1 2 3 4 5 6 7 8 9 None - Dot */
const unsigned char seg_numbers[13]={
	0x28, 0xF9, 0x1C,
	0x58, 0xC9, 0x4A,
	0x0A, 0xF8, 0x08,
	0x48, 0xFF, 0xDF, 0xF7};

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

//variables for displaying lamps on 7-segment sisplay
static volatile uint8_t L_PRC_dpy_data1=0;
static volatile uint8_t L_PRC_dpy_data2=0;
static volatile uint8_t L_PRC_dpy_data3=SEG_SPACE;


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

// previous variables
static volatile bool L_PRC_vbl_lock_rightbutton_prev= false;









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


// Handling bulbs

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
	dpy_trm_s01__7seq_write_3digit(seg_numbers[L_PRC_dpy_data1],seg_numbers[L_PRC_dpy_data2],seg_numbers[L_PRC_dpy_data3]);
}

static void L_PRC_v_window_down_f(void)
{
	L_PRC_dpy_data1=2;
	dpy_trm_s01__7seq_write_3digit(seg_numbers[L_PRC_dpy_data1],seg_numbers[L_PRC_dpy_data2],seg_numbers[L_PRC_dpy_data3]);
}

static void L_PRC_v_window_stop_f(void)
{
	L_PRC_dpy_data1=0;
	dpy_trm_s01__7seq_write_3digit(seg_numbers[L_PRC_dpy_data1],seg_numbers[L_PRC_dpy_data2],seg_numbers[L_PRC_dpy_data3]);
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
	dpy_trm_s01__7seq_write_3digit(seg_numbers[L_PRC_dpy_data1],seg_numbers[L_PRC_dpy_data2],seg_numbers[L_PRC_dpy_data3]);
}

static void L_PRC_v_mirror_left_f(void)
{
	L_PRC_dpy_data2=4;
	dpy_trm_s01__7seq_write_3digit(seg_numbers[L_PRC_dpy_data1],seg_numbers[L_PRC_dpy_data2],seg_numbers[L_PRC_dpy_data3]);
}

static void L_PRC_v_mirror_right_f(void)
{
	L_PRC_dpy_data2=3;
	dpy_trm_s01__7seq_write_3digit(seg_numbers[L_PRC_dpy_data1],seg_numbers[L_PRC_dpy_data2],seg_numbers[L_PRC_dpy_data3]);
}

static void L_PRC_v_mirror_down_f(void)
{
	L_PRC_dpy_data2=2;
	dpy_trm_s01__7seq_write_3digit(seg_numbers[L_PRC_dpy_data1],seg_numbers[L_PRC_dpy_data2],seg_numbers[L_PRC_dpy_data3]);
}

static void L_PRC_v_mirror_up_f(void)
{
	L_PRC_dpy_data2=1;
	dpy_trm_s01__7seq_write_3digit(seg_numbers[L_PRC_dpy_data1],seg_numbers[L_PRC_dpy_data2],seg_numbers[L_PRC_dpy_data3]);
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
		//Window Up
		if(L_PRC_vbl_win_engine_up_control)				L_PRC_v_window_up_f();	
		//Window Down	 
		else if(L_PRC_vbl_win_engine_down_control)		L_PRC_v_window_down_f();
		 
	}
	// Ablak helyi vezérlése (amíg nyomva tartjuk a gombot, addig mozog az ablak)
	else if(L_PRC_vbl_win_engine_up_rightbutton || L_PRC_vbl_win_engine_down_rightbutton)
	{	
		//Window Up
		if(L_PRC_vbl_win_engine_up_rightbutton)				L_PRC_v_window_up_f();
		//Window Down	 
		else if(L_PRC_vbl_win_engine_down_rightbutton)		L_PRC_v_window_down_f();
	
	// Ha nincs vezérlés, az ablak nem mozog
	else	L_PRC_v_window_stop_f();
		

	/************** KÖZPONTI ZÁR ****************/

	// Központi zár vezérlése a bal ajtóról 
	if(L_PRC_vbl_lock_leftbutton)		L_PRC_v_lock_on_f();
	else								L_PRC_v_lock_off_f();	
	
	// Központi zár helyi vezérlése (ha megnyomjuk a gombot a központi zár állapota megváltozik) (felfutó él!!!)
	if((L_PRC_vbl_lock_rightbutton_prev==0) && (L_PRC_vbl_lock_rightbutton==1))
	{
		 if(L_PRC_v_lock_getstatus_f()) L_PRC_v_lock_on_f();
		 else							L_PRC_v_lock_off_f();
	}

	L_PRC_vbl_lock_rightbutton_prev=L_PRC_vbl_lock_rightstatus;

	/************** TÜKÖR FÛTÉS ****************/
		
	//	Tükörfûtés vezérlése a bal ajtóról
	if(L_PRC_vbl_win_mirror_heat_control)	L_PRC_v_heat_on_f();
	else									L_PRC_v_heat_off_f();
	

	
	/************** TÜKÖR MOZGATÁS ****************/

	// Ha a jobb oldali visszapillantó tükör van kiválasztva
	if(L_PRC_vbl_win_mirror_select_control==true)   
	{
		if(L_PRC_vbl_win_mirror_left_control) 			L_PRC_v_mirror_left_f();
		else if(L_PRC_vbl_win_mirror_right_control)		L_PRC_v_mirror_right_f();
		else if(L_PRC_vbl_win_mirror_down_control)		L_PRC_v_mirror_down_f();
		else if(L_PRC_vbl_win_mirror_up_control)		L_PRC_v_mirror_up_f();
		else											L_PRC_v_mirror_stop_f();		
	}
	else	L_PRC_v_mirror_stop_f();


}

/********  Refresh status  ***************************/
void PRC_v_refresh_status_f(void)
{
	// Getting real state of the actuators
	L_PRC_vbl_win_engine_down_status=L_PRC_v_window_up_getstatus_f();
	L_PRC_vbl_win_engine_up_status=L_PRC_v_window_up_getstatus_f();
	L_PRC_vbl_lock_rightstatus= L_PRC_v_lock_getstatus_f();
	L_PRC_vbl_win_mirror_heat_rightstatus=L_PRC_v_heat_getstatus_f();
	L_PRC_vbl_win_mirror_left_status=L_PRC_v_mirror_left_getstatus_f();
	L_PRC_vbl_win_mirror_right_status=L_PRC_v_mirror_right_getstatus_f();
	L_PRC_vbl_win_mirror_down_status=L_PRC_v_mirror_down_getstatus_f();
	L_PRC_vbl_win_mirror_up_status=L_PRC_v_mirror_up_getstatus_f();



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

