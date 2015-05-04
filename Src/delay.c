/*
 * delay.c
 *
 *  Created on: 04 мая 2015 г.
 *      Author: landyrev
 */
#include <msp430.h>
#include <delay.h>
#include <init.h>

void delay(unsigned int ms)
{
	timer0flag=0;
	TA0CTL = 0;
	TA0CCR0=ms*(SMCLK/8)/1000;
	TA0CTL |= TASSEL1 | ID0 | ID1 | MC0 | TACLR | TAIE;
	while (timer0flag==0)
	{
		;
	}

}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void T0INT()
{
	timer0flag=1;
	TA0CTL=0;
}
