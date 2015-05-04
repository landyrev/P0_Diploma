#include <stdio.h>
#include <init.h>
#include <uart.h>
#include <delay.h>
#include <cardreader.h>

int main(void)  //Main program
{
	deviceInit();

	initCardReader();
	initUART();

	__bis_SR_register(GIE); // Enable all interrupts

	while(1)
	{
		UARTsendChar('a');
		UARTsendChar('b');
		UARTsendChar('c');
		UARTsendChar('d');
		__delay_cycles(100000);
	}

	__no_operation();
}
