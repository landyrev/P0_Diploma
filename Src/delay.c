/*
 * delay.c
 *
 *  Created on: 04 мая 2015 г.
 *      Author: landyrev
 */
#include <msp430.h>
#include <delay.h>
#include <init.h>

void delay_micro(unsigned int micros)
{
	timer0flag=0;
	TA0CTL = 0;
	TA0CCR0=micros;
	TA0CTL |= TASSEL1 | MC0 | TACLR | TAIE;
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
