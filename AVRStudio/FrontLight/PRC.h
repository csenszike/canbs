/*
	PRC driver
*/
/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*===========================================================[ HEADER INIT ]=*/
#ifndef PRC_H
#define PRC_H
/*=======================================================[ EXTERNAL MACROS ]=*/
/* macro constants and read macros */

#define PRC_U16_FILTER1		0x0100
#define PRC_U16_FILTER2		0x0100
#define PRC_U16_SENDX20MS	0x0003

/*=====================================================[ EXTERNAL TYPEDEFS ]=*/
/* State Machine Constants */
/*======================================================[ EXTERNAL GLOBALS ]=*/
/* Just for debug !!!!*/

extern CAN_st_message_t PRC_stm_tx_message;

/*=========================================[ EXTPORTED FUNCTION PROTOTYPES ]=*/

extern void PRC_v_init_f(void);
extern void PRC_v_refresh_remote_control_f(void);
extern void PRC_v_refresh_message_status_f(void);
extern void PRC_v_refresh_local_control_f(void);
extern void PRC_v_refresh_local_status_f(void);
extern void PRC_v_process_f(void);
extern void PRC_v_20mstick_f(void);

#endif /* PRC_H */
/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/


