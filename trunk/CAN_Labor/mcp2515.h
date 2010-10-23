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


#ifndef _MCP2515_H_
#define _MCP2515_H_

// Structures

typedef struct
{
    unsigned short int  	id;
    unsigned char   		rtr;
    unsigned char   		length;
    unsigned char   		data[8];
} CAN_message;


// Public FUNCTIONS

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
void mcp2515_init(void);

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
void can_send_standard_message(CAN_message *p_message);

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
void can_receive_message(CAN_message *p_message);

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
void mcp2515_Set_standard_mask_Rx0(unsigned short mask);
void mcp2515_Set_standard_mask_Rx1(unsigned short mask);

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
void mcp2515_Set_standard_filter_RxF0(unsigned short filter);
void mcp2515_Set_standard_filter_RxF1(unsigned short filter);
void mcp2515_Set_standard_filter_RxF2(unsigned short filter);
void mcp2515_Set_standard_filter_RxF3(unsigned short filter);
void mcp2515_Set_standard_filter_RxF4(unsigned short filter);
void mcp2515_Set_standard_filter_RxF5(unsigned short filter);

// Private functions

void mcp2515_reset(void);
void mcp2515_TxBuffern_ready_to_send(unsigned short buffer);
void mcp2515_write(unsigned char adr,unsigned char data);
unsigned char mcp2515_read(unsigned char adr);
unsigned char mcp2515_read_status(void);
unsigned char mcp2515_read_rx_status(void);
void mcp2515_bit_modify(unsigned char adr, unsigned char mask, unsigned char data);


// Az MCP2515 SPI Commands
#define SPI_RESET		0xC0
#define	SPI_READ		0x03
#define	SPI_READ_RX		0x90
#define	SPI_WRITE		0x02
#define	SPI_WRITE_TX	0x40
#define	SPI_RTS			0x80
#define SPI_READ_STATUS	0xA0
#define	SPI_RX_STATUS	0xB0
#define	SPI_BIT_MODIFY	0x05

// Register Addresses

// Rx registers
#define RXF0SIDH	0x00
#define RXF0SIDL	0x01
#define RXF0EID8	0x02
#define RXF0EID0	0x03
#define RXF1SIDH	0x04
#define RXF1SIDL	0x05
#define RXF1EID8	0x06
#define RXF1EID0	0x07
#define RXF2SIDH	0x08
#define RXF2SIDL	0x09
#define RXF2EID8	0x0A
#define RXF2EID0	0x0B
#define BFPCTRL		0x0C
#define TXRTSCTRL	0x0D
#define CANSTAT		0x0E
#define CANCTRL		0x0F

#define RXF3SIDH	0x10
#define RXF3SIDL	0x11
#define RXF3EID8	0x12
#define RXF3EID0	0x13
#define RXF4SIDH	0x14
#define RXF4SIDL	0x15
#define RXF4EID8	0x16
#define RXF4EID0	0x17
#define RXF5SIDH	0x18
#define RXF5SIDL	0x19
#define RXF5EID8	0x1A
#define RXF5EID0	0x1B
#define TEC			0x1C
#define REC         0x1D

#define RXM0SIDH	0x20
#define RXM0SIDL	0x21
#define RXM0EID8	0x22
#define RXM0EID0	0x23
#define RXM1SIDH	0x24
#define RXM1SIDL	0x25
#define RXM1EID8	0x26
#define RXM1EID0	0x27
#define CNF3		0x28
#define CNF2		0x29
#define CNF1		0x2A
#define CANINTE		0x2B
#define CANINTF		0x2C
#define EFLG		0x2D

// Tx regiszters

//Tx Buffer 0
#define TXB0CTRL	0x30
#define TXB0SIDH	0x31
#define TXB0SIDL	0x32
#define TXB0EID8	0x33
#define TXB0EID0	0x34
#define TXB0DLC		0x35
#define TXB0D0		0x36
#define TXB0D1		0x37
#define TXB0D2		0x38
#define TXB0D3		0x39
#define TXB0D4		0x3A
#define TXB0D5		0x3B
#define TXB0D6		0x3C
#define TXB0D7		0x3D

#define TXB1CTRL	0x40
#define TXB1SIDH	0x41
#define TXB1SIDL	0x42
#define TXB1EID8	0x43
#define TXB1EID0	0x44
#define TXB1DLC		0x45
#define TXB1D0		0x46
#define TXB1D1		0x47
#define TXB1D2		0x48
#define TXB1D3		0x49
#define TXB1D4		0x4A
#define TXB1D5		0x4B
#define TXB1D6		0x4C
#define TXB1D7		0x4D

#define TXB2CTRL	0x50
#define TXB2SIDH	0x51
#define TXB2SIDL	0x52
#define TXB2EID8	0x53
#define TXB2EID0	0x54
#define TXB2DLC		0x55
#define TXB2D0		0x56
#define TXB2D1		0x57
#define TXB2D2		0x58
#define TXB2D3		0x59
#define TXB2D4		0x5A
#define TXB2D5		0x5B
#define TXB2D6		0x5C
#define TXB2D7		0x5D

#define RXB0CTRL	0x60
#define RXB0SIDH	0x61
#define RXB0SIDL	0x62
#define RXB0EID8	0x63
#define RXB0EID0	0x64
#define RXB0DLC		0x65
#define RXB0D0		0x66
#define RXB0D1		0x67
#define RXB0D2		0x68
#define RXB0D3		0x69
#define RXB0D4		0x6A
#define RXB0D5		0x6B
#define RXB0D6		0x6C
#define RXB0D7		0x6D

#define RXB1CTRL	0x70
#define RXB1SIDH	0x71
#define RXB1SIDL	0x72
#define RXB1EID8	0x73
#define RXB1EID0	0x74
#define RXB1DLC		0x75
#define RXB1D0		0x76
#define RXB1D1		0x77
#define RXB1D2		0x78
#define RXB1D3		0x79
#define RXB1D4		0x7A
#define RXB1D5		0x7B
#define RXB1D6		0x7C
#define RXB1D7		0x7D


//	Bitdefinition der verschiedenen Register
//	Bitdefinition von BFPCTRL

#define B1BFS		5
#define B0BFS		4
#define B1BFE		3
#define B0BFE		2
#define B1BFM		1
#define B0BFM		0


//	Bitdefinition of TXRTSCTRL

#define B2RTS		5
#define B1RTS		4
#define B0RTS		3
#define B2RTSM		2
#define B1RTSM		1
#define B0RTSM		0

// 	Bitdefinition von CANSTAT

#define OPMOD2		7
#define OPMOD1		6
#define OPMOD0		5
#define ICOD2		3
#define ICOD1		2
#define ICOD0		1


//	Bitdefinition von CANCTRL

#define REQOP2		7
#define REQOP1		6
#define REQOP0		5
#define ABAT		4
#define OSM			3
#define CLKEN		2
#define CLKPRE1		1
#define CLKPRE0		0


//	Bitdefinition von CNF3

#define WAKFIL		6
#define PHSEG22		2
#define PHSEG21		1
#define PHSEG20		0


//	Bitdefinition von CNF2

#define BTLMODE		7
#define SAM			6
#define PHSEG12		5
#define PHSEG11		4
#define PHSEG10		3
#define PHSEG2		2
#define PHSEG1		1
#define PHSEG0		0


//	Bitdefinition von CNF1

#define SJW1		7
#define SJW0		6
#define BRP5		5
#define BRP4		4
#define BRP3		3
#define BRP2		2
#define BRP1		1
#define BRP0		0


//	Bitdefinition von CANINTE

#define MERRE		7
#define WAKIE		6
#define ERRIE		5
#define TX2IE		4
#define TX1IE		3
#define TX0IE		2
#define RX1IE		1
#define RX0IE		0


//	Bitdefinition von CANINTF

#define MERRF		7
#define WAKIF		6
#define ERRIF		5
#define TX2IF		4
#define TX1IF		3
#define TX0IF		2
#define RX1IF		1
#define RX0IF		0


//	Bitdefinition von EFLG

#define RX1OVR		7
#define RX0OVR		6
#define TXB0		5
#define TXEP		4
#define RXEP		3
#define TXWAR		2
#define RXWAR		1
#define EWARN		0


//	Bitdefinition of TXBnCTRL ( n = 0, 1, 2 )

#define ABTF		6
#define MLOA		5
#define TXERR		4
#define TXREQ		3
#define TXP1		1
#define TXP0		0


//	Bitdefinition von RXB0CTRL

#define RXM1		6
#define RXM0		5
#define RXRTR		3
#define BUKT		2
#define BUKT1		1
#define FILHIT0		0


//	Bitdefinition of TXBnSIDL ( n = 0, 1 )

#define	EXIDE		3

//	Bitdefinition von RXB1CTRL
//	RXM1, RXM0, RXRTR und FILHIT0 sind schon für RXB0CTRL definiert

#define FILHIT2		2
#define FILHIT1		1


//	Bitdefinition von RXBnSIDL ( n = 0, 1 )

#define	SRR			4
#define	IDE			3

//	Bitdefinition of RXBnDLC ( n = 0, 1 )
//	TXBnDLC   ( same deffs for TxBnDLC also )

#define	RTR			6
#define	DLC3		3
#define	DLC2		2
#define	DLC1		1
#define DLC0		0

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




#endif //_MCP2515_
