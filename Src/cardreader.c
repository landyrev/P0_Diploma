/*
 * cardreader.c
 *
 *  Created on: 04 мая 2015 г.
 *      Author: landyrev
 */

#include <cardreader.h>
#include <init.h>
#include <msp430.h>
#include <uart.h>

void initCardReader()
{
	TB0CTL = 0;
	TA1CCR0=20*(SMCLK/4)/1000;
	TB0CTL |= TBSSEL1 | ID1 | MC0 | TBCLR | TBIE;

	P1DIR &= ~BIT3; //P1.3 Direction IN
	P1DIR |= BIT4; //PP1.4 Direction OUT

	P1OUT |= BIT4;

	P1IES |= BIT3; 	//P1.3 Rising edge Interrupt
	P1IFG &= ~BIT3;	//Clear P1.3 Interrupt Flag
	P1IE |= BIT3; 	//Enable P1.3 Interrupt
}

#pragma vector=PORT1_VECTOR
__interrupt void P1INT() // Обработчик прерывания
{
	P1IE &= ~BIT3; // Запрет прерываний на P1.3
	//Begin of user code

	unsigned short ticks = TB0R;
	TB0CTL = 0;
	TB0CCR0=20*(SMCLK/4)/1000;
	TB0CTL |= TBSSEL1 | ID1 | MC0 | TBCLR | TBIE;

	if (timeoutFlag)
	{
		timeoutFlag=0;
		cardID=1;
		cardIDnum=1;
	} else
	{
		unsigned short count = ticks/((SMCLK/4)/1000)+0.4;
		while(count>1)
		{
			cardID<<=1;
			count--;
			cardIDnum++;
		}
		cardIDnum++;
		cardID<<=1;
		cardID |= 1;
		if (cardIDnum>=26)
		{
			cardID=~cardID>>1;
			cardID&=0xFFFF;
			UARTsendID("cardID",(short)cardID);
		}

	}


	//End of user code
	P1IE |= BIT3; // Разрешение прерываний на P1.2
	P1IFG &= ~BIT3; // Очистка флага прерываний для P1.2
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void T1INT()
{
	timeoutFlag=1;
	TB0CTL &= ~TBIFG;
}
