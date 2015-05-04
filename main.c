#include <msp430.h> 
#include <stdio.h>

void delay(unsigned int ms);

int main(void)  //Main program
{
   WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

   CSCTL0_H = 0xA5; //Control word
   CSCTL3 = 0;	//Set all dividers to 0
   CSCTL3 |= DIVS0 | DIVS2; //Set SMCLK Divider to 8

   P1DIR &= ~BIT3; //P1.3 Direction IN
   PJDIR |= BIT1; //LED1 Direction OUT

   PJOUT &= ~BIT1; //LED1 Off

   P1IES |= BIT3; 	//P1.3 Rising edge Interrupt
   P1IFG &= ~BIT3;	//Clear P1.3 Interrupt Flag
   P1IE |= BIT3; 	//Enable P1.3 Interrupt

   __bis_SR_register(GIE); // Enable all interrupts

   while(1)
   {
	   PJOUT ^= BIT1;
	   delay(1000);
	   PJOUT ^= BIT1;
	   delay(1000);
   }
   __no_operation();
}

#pragma vector=PORT1_VECTOR
__interrupt void P1INT() // Обработчик прерывания
{
	P1IE &= ~BIT3; // Запрет прерываний на P1.3
	//Begin of user code
	PJOUT |= BIT1;
	__delay_cycles(100);
	PJOUT &= ~BIT1;
	//End of user code
	P1IE |= BIT3; // Разрешение прерываний на P1.2
	P1IFG &= ~BIT3; // Очистка флага прерываний для P1.2
}
