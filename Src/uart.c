/*
 * uart.c
 *
 *  Created on: 04 мая 2015 г.
 *      Author: landyrev
 */
#include <init.h>
#include <uart.h>

void initUART()
{
	// Configure UART pins
	P2SEL1 |= BIT0 + BIT1;
	P2SEL0 &= ~(BIT0 + BIT1);
	// Configure UART 0
	UCA0CTL1 |= UCSWRST;
	UCA0CTL1 = UCSSEL1;                      // Set SMCLK = 250000 as UCBRCLK
	UCA0BR0 = 26;                              // 9600 baud
	UCA0BR1 = 0;
	UCA0MCTLW |= 0x0000;                  // 250000/9600 - INT(250000/9600)=0.41
										  // UCBRSx value = 0x53 (See UG)
	UCA0CTL1 &= ~UCSWRST;                     // release from reset
	//UCA0IE |= UCRXIE;                         // Enable RX interrupt
}

void UARTsendChar (char ch)
{
	UCA0TXBUF_L=ch;
}
