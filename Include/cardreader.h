/*
 * cardreader.h
 *
 *  Created on: 04 мая 2015 г.
 *      Author: landyrev
 */

unsigned short timeoutFlag;
volatile unsigned long cardID;
unsigned short cardIDnum;

void initCardReader();
