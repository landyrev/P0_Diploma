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
	UARTsemaphore=1;
	// Configure UART pins
	P2SEL1 |= BIT0 + BIT1;
	P2SEL0 &= ~(BIT0 + BIT1);

	PJDIR |= BIT0+BIT1+BIT2+BIT3;
	PJOUT &= ~(BIT0+BIT1+BIT2+BIT3);

	// Configure UART 0
	UCA0CTL1 |= UCSWRST;
	UCA0CTL1 = UCSSEL1;                      // Set SMCLK = 250000 as UCBRCLK
	UCA0BR0 = 6;                              // 9600 baud
	UCA0BR1 = 0;
	UCA0MCTLW=0;
	UCA0MCTLW_L |= UCBRF3_L;				//UCBRFx = 8(0b1000)
	UCA0MCTLW_H |= 0x20;					//UCBRSx = 0x20
	UCA0MCTLW_L |= UCOS16_L;				//UCOS16=1;

	UCA0CTL1 &= ~UCSWRST;                     // release from reset
}

void UARTsemSet()
{
	while (!UARTsemaphore);
	UARTsemaphore--;
}

void UARTsemLeave()
{
	UARTsemaphore++;
}

void UARTsendChar (unsigned char ch)
{
	while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
	UCA0TXBUF_L=ch;
}

void UARTsend(const unsigned char *_ptr)
{
	unsigned short i=0;
	while (_ptr[i])
		UARTsendChar(_ptr[i++]);
	UARTsendChar(0x0D);
	UARTsendChar(0x0A);
}

void UARTsendID(const unsigned char *_id, const unsigned short number)
{
	UARTsemSet();
	unsigned short i=0;
	while (_id[i])
		UARTsendChar(_id[i++]);
	UARTsendChar(':');
	UARTsendChar((unsigned char)(number>>8));
	UARTsendChar((unsigned char)number);
	UARTsendChar(0x0D);
	UARTsendChar(0x0A);
	UARTsemLeave();
}

void UARTsendLen(const unsigned char *_data, unsigned short length)
{
	UARTsemSet();
	while (length--)
		UARTsendChar(*_data++);
	UARTsemLeave();
}

int UARTRecieveMicro(unsigned int micros, unsigned char* c, int count)
{
	int i;
	for (i=0; i<count; i++)
	{
		if (UARTgetCharMicro(micros, &c[i]))
			return 1;
	}
	return 0;
}

int UARTgetCharMicro(unsigned int micros, unsigned char* c)
{
	TA0CTL = 0;
	TA0CCR0=micros;
	TA0CTL |= TASSEL1 | MC0 | TACLR;
	while (!( (TA0CTL&TAIFG)||(UCA0IFG&UCRXIFG) ));
	TA0CTL = 0;
	if (UCA0IFG&UCRXIFG)
	{
		*c=UCA0RXBUF;
		return 0;
	} else
	{
		*c=0;
		return 1;
	}
}
