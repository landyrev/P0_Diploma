#include <msp430.h>
#include <init.h>

int deviceInit()
{
	   WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

	   //Setting Clock
	   CSCTL0_H = 0xA5; //Control word
	   CSCTL3 = 0;	//Set all dividers to 0
	   CSCTL3 |= DIVS0 | DIVS1; //Set SMCLK Divider to 8
	   return 0;
}
