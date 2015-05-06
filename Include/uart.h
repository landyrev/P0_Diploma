/*
 * uart.h
 *
 *  Created on: 04 мая 2015 г.
 *      Author: landyrev
 */

#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

void initUART();
void UARTsendChar (char ch);
void UARTsend(const char *_str);
void UARTsendID(const char *_id, const unsigned short number);
void UARTsemSet();
void UARTsemLeave();

unsigned short UARTsemaphore;

#endif /* INCLUDE_UART_H_ */
