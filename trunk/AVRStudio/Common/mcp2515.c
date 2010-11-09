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
 

/*==============================================================[ INCLUDES ]=*/

#include "platform.h"

#include "mcp2515.h"
	
/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

// definition of SPI chip select pin
#define MCP2515_CS_DIR_OUTPUT() 	GPIO_13_DIR_OUTPUT()	
#define MCP2515_CS_CLEAR() 			GPIO_13_CLEAR()	
#define MCP2515_CS_SET()			GPIO_13_SET()

// definition of SPI chip select pin
#define MCP2515_INT_DIR_INPUT() 	GPIO_12_DIR_INPUT()	
#define MCP2515_INT_GET_VALUE() 	GPIO_12_GET_VALUE()


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

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*======================================================[ INTERNAL GLOBALS ]=*/

/*======================================================[ EXTERNAL GLOBALS ]=*/

volatile CAN_message rx_message;
volatile bool New_message_flag=false;

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

ISR(SIG_INTERRUPT5);
static void mcp2515_reset(void);
static void mcp2515_TxBuffern_ready_to_send(unsigned short buffer);
static void mcp2515_write(unsigned char adr,unsigned char data);
static unsigned char mcp2515_read(unsigned char adr);
static unsigned char mcp2515_read_status(void);
static unsigned char mcp2515_read_rx_status(void);
static void mcp2515_bit_modify(unsigned char adr, unsigned char mask, unsigned char data);
static void INT5_IT_ENABLE(void);
static void INT5_IT_DISABLE(void);

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

/********  External Interrupt Request 0 ISR  ****************/
ISR(SIG_INTERRUPT5)
{

	uint8_t data,i;
	
	MCP2515_CS_CLEAR();
		SPI_byte(SPI_READ_RX,0);	// Read Buffer Command

		// Standard ID kiolvasása
		SPI_byte(0,&data);			
		rx_message.id =  ((unsigned int) data) << 3;
		SPI_byte(0,&data);			
		rx_message.id |= ((unsigned int) data) >> 5;
		
		SPI_byte(0,&data);			
		SPI_byte(0,&data);			

		// Length
		SPI_byte(0,&data);
		rx_message.length = data&0xf;

		// Adatok kiolvasása
		for (i=0;i<rx_message.length;i++) {
			SPI_byte(0,&data);
			rx_message.data[i] = data;
		}
		
		// Setting the new message flag
		New_message_flag=true;

	// Clearing the receive interrupt by clearing the RXnIF bit.
	mcp2515_bit_modify(CANINTF, (1<<RX0IF), 0);
}

/********  function  *****************/
static void mcp2515_reset(void)
{
	MCP2515_CS_CLEAR(); 
	SPI_byte(SPI_RESET,0);
	MCP2515_CS_SET();
}

/********  function  *****************/
static void mcp2515_TxBuffern_ready_to_send(unsigned short buffer)
{
	if(buffer<3)
	{
		MCP2515_CS_CLEAR();
			SPI_byte(SPI_RTS | (1<<buffer),0);	// Ready to Send command
		MCP2515_CS_SET();
	}
}

/********  function  *****************/
static void mcp2515_write(unsigned char adr,unsigned char data)
{
		MCP2515_CS_CLEAR();
			SPI_byte(SPI_WRITE,0);	// Writing Command
			SPI_byte(adr,0);		// Register Address
			SPI_byte(data,0);		// Data
		MCP2515_CS_SET();
}

/********  function  *****************/
static unsigned char mcp2515_read(unsigned char adr)
{
	unsigned char read;
	
	MCP2515_CS_CLEAR();		
		SPI_byte(SPI_READ,0);	// Read command
		SPI_byte(adr,0);		// Register Address
		SPI_byte(0,&read);		// Data
	MCP2515_CS_SET();	
	return read;
}

/********  function  *****************/
static unsigned char mcp2515_read_status(void)
{

	unsigned char read;
		
		MCP2515_CS_CLEAR();		
			SPI_byte(SPI_READ_STATUS,0);	// Read command
			SPI_byte(0,&read);			// Status
			SPI_byte(0,&read);			// Status repeated
		MCP2515_CS_SET();	

	return read;
	
}

/********  function  *****************/
static unsigned char mcp2515_read_rx_status(void)
{
	
	unsigned char read;
		
		MCP2515_CS_CLEAR();		
			SPI_byte(SPI_RX_STATUS,0);	// Read command
			SPI_byte(0,&read);			// Status
			SPI_byte(0,&read);			// Status repeated
		MCP2515_CS_SET();	

	return read;
	
}


/********  function  *****************/
static void mcp2515_bit_modify(unsigned char adr, unsigned char mask, unsigned char data)
{

		MCP2515_CS_CLEAR();		
			SPI_byte(SPI_READ,0);	// Read command
			SPI_byte(adr,0);		// Register Address
			SPI_byte(mask,0);		// Mask for bit modification
			SPI_byte(data,0);		// data
		MCP2515_CS_SET();	
	
}

/********  function  *****************/
static void INT5_IT_ENABLE(void)
{
	EIFR =(1<<INTF5);
	EICRB=(1<<ISC51); // The falling edge of INT5 generates an interrupt request
	EIMSK=(1<<INT5); // INT5: External Interrupt Request 5 Enable
}


/********  function  *****************/
static void INT5_IT_DISABLE(void)
{
    EIMSK=0;         // External Interrupt Request disable
}

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

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


void mcp2515_init(void)
{

MCP2515_CS_DIR_OUTPUT();
MCP2515_INT_DIR_INPUT();

#if ARM_LPC2106	
	SPI_Init(0,0,0,20);
#elif AVR_ATMEGA128
	SPI_Init(0,0,0,4);
#endif


// Reseting the CAN controller
	mcp2515_reset();
	
// Disabling not used bits
	mcp2515_write( BFPCTRL, 0 ); // Deactivating RXnBF Pins
	mcp2515_write( TXRTSCTRL, 0 ); // Setting TXnRTS Bits Input
		
	mcp2515_write(CNF1,R_CNF1_500kbps);// Baudrate Prescaler setup
	mcp2515_write(CNF2,R_CNF2_500kbps);// Propagation Segment and phase 1 setup
	mcp2515_write(CNF3,R_CNF3_500kbps);// Phase 2 setup
	
	mcp2515_write( RXB0CTRL, 0 ); // Receives messages enabled by filter 0, no rollower alloved
	mcp2515_write( RXB1CTRL, 0 ); // Receives messages enabled by filter
	
	// Clearing the mask every message is accepted
	mcp2515_write( RXM1SIDH, 0 );
	mcp2515_write( RXM1SIDL, 0 );
	mcp2515_write( RXM1EID8, 0 );
	mcp2515_write( RXM1EID0, 0 );
	
	// Setting the mask every message is machd to a filter
//	mcp2515_write( RXM0SIDH, 0xFF );
//	mcp2515_write( RXM0SIDL, 0xFF );
//	mcp2515_write( RXM0EID8, 0xFF );
//	mcp2515_write( RXM0EID0, 0xFF );

	CAN_v_mcp2515_Set_standard_mask_Rx0_f(0x07FF);		// Mask beállítása
//	CAN_v_mcp2515_Set_standard_filter_RxF0_f(0x0100);	// Filter beállítása???
	CAN_v_mcp2515_Set_standard_filter_RxF1_f(0x01E0);	// Filter beállítása???
	
	mcp2515_write( CANINTE, (1<<RX0IE));	// Setting IT control
	
	mcp2515_write( CANCTRL,(1 << OSM));  // Normal mode, One shot enabled	
}



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
void can_receive_message(CAN_message *p_message)
{
	unsigned char data,i;
	while(MCP2515_INT_GET_VALUE());
	
	MCP2515_CS_CLEAR();
		SPI_byte(SPI_READ_RX,0);	// Read Buffer Command

		// Standard ID kiolvasása
		SPI_byte(0,&data);			
		p_message->id =  ((unsigned int) data) << 3;
		SPI_byte(0,&data);			
		p_message->id |= ((unsigned int) data) >> 5;
		
		SPI_byte(0,&data);			
		SPI_byte(0,&data);			

		// Length
		SPI_byte(0,&data);
		p_message->length = data&0xf;

		// Adatok kiolvasása
		for (i=0;i<p_message->length;i++) {
			SPI_byte(0,&data);
			p_message->data[i] = data;
		}
// When a message is moved into either of the receive buffers, the appropriate CANINTF.RXnIF bit is set
// This bit must be cleared by the MCU in order to allow a new message to be received into the buffer
// The receive interrupt is cleared too by clearing the RXnIF bit.
	mcp2515_bit_modify(CANINTF, (1<<RX0IF), 0);   
}

/********  can_receive_message_ISR_ENABLE ******************************************/
void can_receive_message_ISR_ENABLE(void)
{
	INT5_IT_ENABLE();
	mcp2515_bit_modify(CANINTF, (1<<RX0IF), 0);
}

/********  can_receive_message_ISR_ENABLE ******************************************/
void can_receive_message_ISR_DISABLE(void)
{
	INT5_IT_DISABLE();
}


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

void can_send_standard_message(CAN_message *p_message)
{
     unsigned char length = p_message->length;
     unsigned char i;

     // Setting the ID:
     mcp2515_write(TXB0SIDH, (unsigned char)((p_message->id >> 3)&0xFF));
     mcp2515_write(TXB0SIDL, (unsigned char)((p_message->id &0x7)<<5));
     // If the message is RTR
     if (p_message->rtr)
     {
          // RTR message has no data
          mcp2515_write(TXB0DLC, (1<<RTR) | length);
     }
     else
     {
          // Setting data length parameter
          mcp2515_write(TXB0DLC, length);
          // Copying data
          for (i=0;i<length;i++) {
               mcp2515_write(TXB0D0 + i, p_message->data[i]);
          }
     }

     // Sending CAN Message
	 mcp2515_TxBuffern_ready_to_send(0);
     
}


/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_mask_Rx0(unsigned short mask)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short mask: New mask value of RB0 
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

void mcp2515_Set_standard_mask_Rx0(unsigned short mask)
{
	mcp2515_write( RXM0SIDH, (unsigned char)((mask >> 3)&0xFF) );
	mcp2515_write( RXM0SIDL, (unsigned char)((mask&0x7)<<5) );

}


/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_mask_Rx1(unsigned short mask)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short mask: New mask value of RB1
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
void mcp2515_Set_standard_mask_Rx1(unsigned short mask)
{
	mcp2515_write( RXM1SIDH, (unsigned char)((mask >> 3)&0xFF) );
	mcp2515_write( RXM1SIDL, (unsigned char)((mask&0x7)<<5) );

}



/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_filter_RxF0(unsigned short filter)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short filter: New filter value of RxF0
 *			
 * Output:        none  
 *
 * Side Effects:  Set the receiver filter value
 *					
 * Overview:      Set the receiver filter value
 *
 * Note:          none
  ********************************************************************/
void mcp2515_Set_standard_filter_RxF0(unsigned short filter)
{
	mcp2515_write( RXF0SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF0SIDL, (unsigned char)((filter&0x7)<<5) );

}


/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_filter_RxF1(unsigned short filter)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short filter: New filter value of RxF1
 *			
 * Output:        none  
 *
 * Side Effects:  Set the receiver filter value
 *					
 * Overview:      Set the receiver filter value
 *
 * Note:          none
  ********************************************************************/
void mcp2515_Set_standard_filter_RxF1(unsigned short filter)
{
	mcp2515_write( RXF1SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF1SIDL, (unsigned char)((filter&0x7)<<5) );

}



/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_filter_RxF2(unsigned short filter)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short filter: New filter value of RxF2
 *			
 * Output:        none  
 *
 * Side Effects:  Set the receiver filter value
 *					
 * Overview:      Set the receiver filter value
 *
 * Note:          none
  ********************************************************************/
void mcp2515_Set_standard_filter_RxF2(unsigned short filter)
{
	mcp2515_write( RXF2SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF2SIDL, (unsigned char)((filter&0x7)<<5) );

}



/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_filter_RxF3(unsigned short filter)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short filter: New filter value of RxF3
 *			
 * Output:        none  
 *
 * Side Effects:  Set the receiver filter value
 *					
 * Overview:      Set the receiver filter value
 *
 * Note:          none
  ********************************************************************/
void mcp2515_Set_standard_filter_RxF3(unsigned short filter)
{
	mcp2515_write( RXF3SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF3SIDL, (unsigned char)((filter&0x7)<<5) );

}



/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_filter_RxF4(unsigned short filter)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short filter: New filter value of RxF
 *			
 * Output:        none  
 *
 * Side Effects:  Set the receiver filter value
 *					
 * Overview:      Set the receiver filter value
 *
 * Note:          none
  ********************************************************************/
void mcp2515_Set_standard_filter_RxF4(unsigned short filter)
{
	mcp2515_write( RXF4SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF4SIDL, (unsigned char)((filter&0x7)<<5) );

}



/*********************************************************************
 * Function: 	  void mcp2515_Set_standard_filter_RxF5(unsigned short filter)
 *
 * PreCondition:  The inicialisation of the MCP2515 CAN controller
 *     
 * Input:         unsigned short filter: New filter value of RxF5
 *			
 * Output:        none  
 *
 * Side Effects:  Set the receiver filter value
 *					
 * Overview:      Set the receiver filter value
 *
 * Note:          none
  ********************************************************************/
void mcp2515_Set_standard_filter_RxF5(unsigned short filter)
{
	mcp2515_write( RXF5SIDH, (unsigned char)((filter >> 3)&0xFF) );
	mcp2515_write( RXF5SIDL, (unsigned char)((filter&0x7)<<5) );

}

/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
*/

