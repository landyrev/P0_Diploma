#include <stdio.h>
#include <init.h>
#include <uart.h>
//#include <cardreader.h>

int main(void)  //Main program
{
	deviceInit();
	puts("Shiet");

	//initCardReader();
	initUART();

	__bis_SR_register(GIE); // Enable all interrupts

	__no_operation();
}
