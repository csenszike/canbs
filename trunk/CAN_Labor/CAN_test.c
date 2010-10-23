/**
*	Simple example program for using the MCP2515 CAN interface
**/

#include "platform.h"
#include "mcp2515.h"
#include "dpy_trm_s01.h"

void main()
{
CAN_message message,rx_message;

dpy_trm_s01__Init(); 	// A kijelzõ panel iniciálása
mcp2515_init();			// A CAN kommunikáció iniciálása


message.id = 0x0123;
message.rtr = 0;
message.length = 2;
message.data[0] = 0;
message.data[1] = 0;	// Egy CAN üzenet összeállítása

while(1)
{

	message.data[0]++;
	can_send_standard_message(&message);	// CAN üzenetküldés
	_delay_ms(50);
	can_receive_message(&rx_message);		// CAN üzenetfogadás
	dpy_trm_s01__7seq_write_number(rx_message.data[2], 0);


}

}
