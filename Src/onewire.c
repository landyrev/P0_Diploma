/*
 * onewire.c
 *
 *  Created on: 30 мая 2015 г.
 *      Author: landyrev
 */

#include <onewire.h>
#include <init.h>
#include <delay.h>

unsigned int OWreset()
{
	P1DIR |= BIT6; P1OUT &= ~BIT6;
	delay_micro(480);
	P1DIR &= ~BIT6;
	delay_micro(60);
	if(P1IN&BIT6)
		return 0;
	else
	{
		delay_micro(150);
		return 1;
	}
}

void OWputch(char c)
{
	int i;
	for (i=0; i<8; i++)
	{
		P1DIR |= BIT6;
		unsigned int d=(c>>i)&1;
		if (((c>>i)&1)==0)
		{
			P1OUT &= ~BIT6;
			delay_micro(61);
			P1DIR &= ~BIT6;
		} else
		{
			P1OUT &= ~BIT6;
			delay_micro(1);
			P1DIR &= ~BIT6;
			delay_micro(59);
		}
		delay_micro(1);
	}
}

char OWreadch()
{
	int i;
	char c=0;
	for (i=0; i<8; i++)
	{
		P1DIR |= BIT6; P1OUT &= ~BIT6;
		delay_micro(1);
		P1DIR &= ~BIT6;
		delay_micro(8);
		if (((P1IN>>6)&1)!=0)
		{
			c |= (1<<i);
		}
		delay_micro(45);
	}
	return c;
}
