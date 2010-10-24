/*********************************************************************
 *
 *              Application Programming Interface (API) 
 *                     for MCP2515 CAN controller
 *
 *********************************************************************
 * FileName:        mcp2515.c
 * Dependencies:    mcp2515.h, platform.h 
 * Processor:       processor independent
 * Complier:        compiler independent              
 * Company:         Budapest University of Technology and Economics
 * 					Department of Measurement and Information Systems
 * 
 * note:
 *	General API for the MCP2515 CAN controller used on the CAN/LIN extension
 *  board of mitmót system
 *
 * Author               Date    Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Balazs Scherer     3/04/07   Original
 ********************************************************************/

/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*===========================================================[ HEADER INIT ]=*/

#ifndef _MCP2515_H_
#define _MCP2515_H_

/*=======================================================[ EXTERNAL MACROS ]=*/
/* macro constants and read macros */

// 8MHz Oscillator settings 
// CAN Bitrate 125 kbps
#define R_CNF1_125kbps	(1<<BRP1)|(1<<BRP0)
#define R_CNF2_125kbps	(1<<BTLMODE)|(1<<PHSEG11)
#define R_CNF3_125kbps	(1<<PHSEG21)

// CAN Bitrate 250 kbps
#define R_CNF1_250kbps	(1<<BRP0)
#define R_CNF2_250kbps	(1<<BTLMODE)|(1<<PHSEG11)
#define R_CNF3_250kbps	(1<<PHSEG21)

// CAN Bitrate 500 kbps
#define R_CNF1_500kbps	0
#define R_CNF2_500kbps	(1<<BTLMODE)|(1<<PHSEG11)
#define R_CNF3_500kbps	(1<<PHSEG21)

/*=====================================================[ EXTERNAL TYPEDEFS ]=*/
/* State Machine Constants */

typedef struct
{
    unsigned short int  	id;
    unsigned char   		rtr;
    unsigned char   		length;
    unsigned char   		data[8];
} CAN_message;

/*======================================================[ EXTERNAL GLOBALS ]=*/

extern CAN_message rx_message;
extern volatile bool New_message_flag;

/*=========================================[ EXTPORTED FUNCTION PROTOTYPES ]=*/

/*********************************************************************
 * Function: 	  void mcp2515_init(void)
 *
 * PreCondition:  none
 *     
 * Input:         none  
 *
 * Output:        none  
 *
 * Side Effects:  Inicialisation of MCP2515 transciever and its related mitmót pins  
 *					
 * Overview:      Call the appropirate GPIO_x_DIR_OUTPUT, GPIO_x_DIR_INPUT
 * 				  functions set the baudrate of the CAN controller to 500kbaud
 *				  clear the mask of Receive buffer 0, set the mask of RCB1
 *
 * Note:          none
  ********************************************************************/
extern void mcp2515_init(void);

/*********************************************************************
 * Function: 	  void can_receive_message(CAN_message *p_message)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         CAN_message *p_message
 *				  structure containing the received message
 *				  fields of the structure:
 *					unsigned short int  	id;
 *					unsigned char   		rtr;
 *					unsigned char   		length;
 *					unsigned char   		data[8];
 *
 * Output:        none  
 *
 * Side Effects:  Blocks the program execution until a valid CAN message received  
 *					
 * Overview:      Blocks the program execution until a valid CAN message received 
 *
 * Note:          none
  ********************************************************************/
extern void can_receive_message(CAN_message *p_message);

/*********************************************************************
 * Function: 	  void can_send_standard_message(CAN_message *p_message)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         CAN_message *p_message
 *				  structure containing the message to be sent
 *				  fields of the structure:
 *					unsigned short int  	id;
 *					unsigned char   		rtr;
 *					unsigned char   		length;
 *					unsigned char   		data[8];
 *
 * Output:        none  
 *
 * Side Effects:  Send a message to the CAN bus in one shot mode, no retransmittion 
 *					
 * Overview:      Send a message to the CAN bus in one shot mode, no retransmittion
 *
 * Note:          none
  ********************************************************************/
extern void can_send_standard_message(CAN_message *p_message);


/********  can_receive_message_ISR_ENABLE ****************************/
extern void can_receive_message_ISR_ENABLE(void);

/********  can_receive_message_ISR_ENABLE ****************************/
extern void can_receive_message_ISR_DISABLE(void);



/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_mask_Rxn(unsigned short mask)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short mask: New mask value of RBn 
 *						(mask bit value 1 means the use of the filter bit in thar position)
 *
 * Output:        none  
 *
 * Side Effects:  Set the receiver filter mask of the given buffer
 *					
 * Overview:      Set the receiver filter mask of the given buffer
 *
 * Note:          none
  ********************************************************************/
extern void mcp2515_Set_standard_mask_Rx0(unsigned short mask);
extern void mcp2515_Set_standard_mask_Rx1(unsigned short mask);

/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_filter_RxFn(unsigned short filter)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short filter: New filter value of RxFn
 *			
 * Output:        none  
 *
 * Side Effects:  Set the receiver filter value
 *					
 * Overview:      Set the receiver filter value
 *
 * Note:          none
  ********************************************************************/
extern void mcp2515_Set_standard_filter_RxF0(unsigned short filter);
extern void mcp2515_Set_standard_filter_RxF1(unsigned short filter);
extern void mcp2515_Set_standard_filter_RxF2(unsigned short filter);
extern void mcp2515_Set_standard_filter_RxF3(unsigned short filter);
extern void mcp2515_Set_standard_filter_RxF4(unsigned short filter);
extern void mcp2515_Set_standard_filter_RxF5(unsigned short filter);


/* This is for coding conventions */
#define CAN_msg_rx_message							rx_message
#define CAN_vbl_New_message_flag					New_message_flag

#define CAN_st_message_t							CAN_message

#define CAN_v_mcp2515_init_f                        mcp2515_init                    
#define CAN_v_can_receive_message_f                 can_receive_message             
#define CAN_v_can_send_standard_message_f           can_send_standard_message       
#define CAN_v_can_receive_message_ISR_ENABLE_f      can_receive_message_ISR_ENABLE  
#define CAN_v_can_receive_message_ISR_DISABLE_f     can_receive_message_ISR_DISABLE 
#define CAN_v_mcp2515_Set_standard_mask_Rx0_f       mcp2515_Set_standard_mask_Rx0   
#define CAN_v_mcp2515_Set_standard_mask_Rx1_f       mcp2515_Set_standard_mask_Rx1   
#define CAN_v_mcp2515_Set_standard_filter_RxF0_f    mcp2515_Set_standard_filter_RxF0
#define CAN_v_mcp2515_Set_standard_filter_RxF1_f    mcp2515_Set_standard_filter_RxF1
#define CAN_v_mcp2515_Set_standard_filter_RxF2_f    mcp2515_Set_standard_filter_RxF2
#define CAN_v_mcp2515_Set_standard_filter_RxF3_f    mcp2515_Set_standard_filter_RxF3
#define CAN_v_mcp2515_Set_standard_filter_RxF4_f    mcp2515_Set_standard_filter_RxF4
#define CAN_v_mcp2515_Set_standard_filter_RxF5_f    mcp2515_Set_standard_filter_RxF5



#endif //_MCP2515_

/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/

