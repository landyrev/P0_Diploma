/*
 * cardreader.h
 *
 *  Created on: 04 ��� 2015 �.
 *      Author: landyrev
 */

unsigned short timeoutFlag;
volatile unsigned long cardID;
unsigned short cardIDnum;

void initCardReader();
