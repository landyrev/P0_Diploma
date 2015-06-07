#include <init.h>
#include <uart.h>
#include <delay.h>
//#include <cardreader.h>
#include <18b20.h>
#include <bacnet.h>
#include <crc.h>

int main(void)  //Main program
{
	deviceInit();

	//initCardReader();
	initUART();

	__bis_SR_register(GIE); // Enable all interrupts

	P3DIR |= BIT4;
	P3OUT &= ~BIT4;

	unsigned short state=STATE_IDLE;
	//while (1)
	//{
	BACnetRoutine(state);
	//}

	//__no_operation();
}


