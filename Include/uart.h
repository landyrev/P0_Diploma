/*
 * uart.h
 *
 *  Created on: 04 мая 2015 г.
 *      Author: landyrev
 */

#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

void initUART();
void UARTsendChar (unsigned char ch);
void UARTsend(const unsigned char *_str);
void UARTsendID(const unsigned char *_id, const unsigned short number);
int UARTgetCharMicro(unsigned int micros, unsigned char* c);
int UARTRecieveMicro(unsigned int micros, unsigned char* c, int count);
void UARTsemSet();
void UARTsemLeave();
void UARTsendLen(const unsigned char *_data, unsigned short length);

unsigned short UARTsemaphore;

#endif /* INCLUDE_UART_H_ */
