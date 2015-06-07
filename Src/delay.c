/*
 * delay.c
 *
 *  Created on: 04 мая 2015 г.
 *      Author: landyrev
 */
#include <msp430.h>
#include <delay.h>
#include <init.h>

int flag;

void delay_micro(unsigned int micros)
{
	TA0CTL = 0;
	TA0CCR0=micros;
	TA0CTL |= TASSEL1 | MC0 | TAIE | TACLR;
	__bis_SR_register(GIE + LPM0_bits);
	TA0CTL = 0;
}

void delay(unsigned int millis)
{
	flag=0;
	TA0CTL = 0;
	TA0CCR0=millis*125; //125 = (SMCLK/8)/1000 - количество тактов в одну мс.
	TA0CTL |= TASSEL1 | ID0 | ID1 | MC0 | TAIE | TACLR;
	__bis_SR_register(GIE + LPM0_bits);
	TA0CTL = 0;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TA0_ISR()
{
	__bic_SR_register_on_exit(LPM0_bits);
	TA0CTL &= ~ TAIFG;
}
