/*
 * delay.c
 *
 *  Created on: 04 ��� 2015 �.
 *      Author: landyrev
 */
#include <msp430.h>
#include <delay.h>
#include <init.h>

void delay_micro(unsigned int micros)
{
	TA0CTL = 0;
	TA0CCR0=micros;
	TA0CTL |= TASSEL1 | MC0 | TACLR;
	while (!(TA0CTL&TAIFG));
	TA0CTL = 0;
}

void delay(unsigned int millis)
{
	TA0CTL = 0;
	TA0CCR0=millis*125; //125 = (SMCLK/8)/1000 - ���������� ������ � ���� ��.
	TA0CTL |= TASSEL1 | ID0 | ID1 | MC0 | TACLR;
	while (!(TA0CTL&TAIFG));
	TA0CTL = 0;
}
