/**
*	Simple example program for using the MCP2515 CAN interface
**/

#include "platform.h"
#include "mcp2515.h"
#include "dpy_trm_s01.h"

void main()
{
CAN_message message,rx_message;

dpy_trm_s01__Init(); 	// A kijelz� panel inici�l�sa
mcp2515_init();			// A CAN kommunik�ci� inici�l�sa


message.id = 0x0123;
message.rtr = 0;
message.length = 2;
message.data[0] = 0;
message.data[1] = 0;	// Egy CAN �zenet �ssze�ll�t�sa

while(1)
{

	message.data[0]++;
	can_send_standard_message(&message);	// CAN �zenetk�ld�s
	_delay_ms(50);
	can_receive_message(&rx_message);		// CAN �zenetfogad�s
	dpy_trm_s01__7seq_write_number(rx_message.data[2], 0);


}

}
