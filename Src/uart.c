/*
 * uart.c
 *
 *  Created on: 04 мая 2015 г.
 *      Author: landyrev
 */
#include <init.h>
#include <uart.h>
#include <stdio.h>

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
	UCA0BR0 = 26;                              // 9600 baud
	UCA0BR1 = 0;
	UCA0MCTLW |= 0x0000;                  // 250000/9600 - INT(250000/9600)=0.41
	// UCBRSx value = 0x53 (See UG)
	UCA0CTL1 &= ~UCSWRST;                     // release from reset
	UCA0IE |= UCRXIE;                         // Enable RX interrupt
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

void UARTsendChar (char ch)
{
	while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
	UCA0TXBUF_L=ch;
}

void UARTsend(const char *_ptr)
{
	unsigned short i=0;
	while (_ptr[i])
		UARTsendChar(_ptr[i++]);
	UARTsendChar(0x0D);
	UARTsendChar(0x0A);
}

void UARTsendID(const char *_id, const unsigned short number)
{
	UARTsemSet();
	unsigned short i=0;
	while (_id[i])
		UARTsendChar(_id[i++]);
	UARTsendChar(':');
	UARTsendChar((char)(number>>8));
	UARTsendChar((char)number);
	UARTsendChar(0x0D);
	UARTsendChar(0x0A);
	UARTsemLeave();
}

// Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	volatile char x;
	switch(__even_in_range(UCA0IV,0x08))
	{
	case 0:break;                             // Vector 0 - no interrupt
	case 2:                                   // Vector 2 - RXIFG
		x=UCA0RXBUF-0x30;
		UARTsemSet();
		switch (x)
		{
		case 0:
			PJOUT ^= BIT0;
			if (PJOUT&BIT0) UARTsend("lamp0:1"); else UARTsend("lamp0:0");
			break;
		case 1:
			PJOUT ^= BIT1;
			if (PJOUT&BIT1) UARTsend("lamp1:1"); else UARTsend("lamp1:0");
			break;
		case 2:
			PJOUT ^= BIT2;
			if (PJOUT&BIT2) UARTsend("lamp2:1"); else UARTsend("lamp2:0");
			break;
		case 3:
			PJOUT ^= BIT3;
			if (PJOUT&BIT3) UARTsend("lamp3:1"); else UARTsend("lamp3:0");
			break;
		case 7:
			if (PJOUT&BIT0) UARTsend("lamp0:1"); else UARTsend("lamp0:0");
			if (PJOUT&BIT1) UARTsend("lamp1:1"); else UARTsend("lamp1:0");
			if (PJOUT&BIT2) UARTsend("lamp2:1"); else UARTsend("lamp2:0");
			if (PJOUT&BIT3) UARTsend("lamp3:1"); else UARTsend("lamp3:0");
		default: break;
		}
		break;
		case 4:break;                             // Vector 4 - TXIFG
		default: break;
	}
	UARTsemLeave();
}
