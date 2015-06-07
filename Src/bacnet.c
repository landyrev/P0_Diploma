/*
 * bacnet.c
 *
 *  Created on: 31 мая 2015 г.
 *      Author: landyrev
 */
#include <init.h>
#include <uart.h>
#include <bacnet.h>
#include <crc.h>
#include <delay.h>

unsigned short BACnetRoutine(unsigned short state)
{
	switch (state)
	{
	case STATE_IDLE:
		recieveFlag=0;
		UCA0IE |= UCSTTIE;									// Enable RX interrupt
		delay(T_NO_TOKEN);
		if (!recieveFlag)
			return NO_TOKEN;
		if (BACnetGetNPDU())
			return STATE_IDLE;
		if (destinationAddress!=TS)
			return STATE_IDLE;
		switch (frameType)
		{
		case 0:
			return USE_TOKEN;
		case 5:
			//DataNoReply();
			return STATE_IDLE;
		case 6:
			DataNeedReply();
			return STATE_IDLE;
		default:
			return STATE_IDLE;
		}
	default:
		return STATE_IDLE;
	}
}

void DataNoReply()
{
	;
}

void DataNeedReply()
{
	unsigned short i=0;
	unsigned short ObjectId=0;
	unsigned short PropertyId=0;
	unsigned short InvokeId=apdu[2];
	unsigned short ServiceChoice=apdu[3];
	for (i=4; i<frameLength; i++)
	{
		switch (apdu[i])
		{
		case 0x0C:
			ObjectId=apdu[++i];
			break;
		case 0x19:
			PropertyId=apdu[++i];
			break;
		default:
			i++;
			break;
		}
	}

	switch(ServiceChoice)
	{
	case 0x0C:
		AnswerRequest(ObjectId, PropertyId, InvokeId);
		break;
	default:
		break;
	}

}

void AnswerRequest(unsigned short ObjectId, unsigned short PropertyID, unsigned short InvokeId)
{
	//unsigned short len=0;
	switch(ObjectId)
	{
	case 1:
		//len = EncodeACKNPDU(EncodeACKReadPropertyAPDU(InvokeId,ObjectId,PropertyID,6223));
		//UARTsendLen(npdu_out,len);
		break;
	default:
		break;
	}
}

unsigned short EncodeACKReadPropertyAPDU(unsigned short InvokeId, unsigned short ObjectId, unsigned short PropertyID, unsigned short value)
{
	apdu_out[0]=0x30; //PDU type=3
	apdu_out[1]=InvokeId;	//InvokeId
	apdu_out[2]=0x0C;	//ServiceChoice

	apdu_out[3]=0x0C;	//ObIdOpenTag
	apdu_out[4]=ObjectId;
	apdu_out[5]=0x19;	//PropertyId
	apdu_out[6]=0x55;	//PRESETN_VALUE
	apdu_out[7]=0x3E;	//Opening Value tag
	apdu_out[8]=0x04;	//Unsigned short
	apdu_out[9]=0;
	apdu_out[10]=0;
	apdu_out[11]=(char)(value>>8);
	apdu_out[12]=(char)(value);
	apdu_out[13]=0x3F;	//Closing tag
	return 14;
}

unsigned short EncodeACKNPDU(unsigned short length)
{
	npdu_out[0]=0x55;
	npdu_out[1]=0xFF;
	npdu_out[2]=0x06;	//Without Reply
	npdu_out[3]=sourceAddress;
	npdu_out[4]=TS;
	npdu_out[5]=(char)(length>>8);
	npdu_out[6]=(char)length;
	npdu_out[7]=Crc8(&npdu_out[2],7);
	int i=0;
	for (i=0; i<length; i++)
		npdu_out[8+i]=apdu_out[i];
	unsigned short crc=Crc16(&apdu[0],length);
	npdu_out[8+length]=(char)(crc>>8);
	npdu_out[8+length+1]=(char)crc;
	return 8+length+2;
}

int BACnetGetNPDU()
{
	unsigned char preamble[2];
	unsigned char header[6];


	if (UARTRecieveMicro(FRAMEGAP,&preamble[0],2))
		return TIMEOUT;

	if ( (preamble[0]!=0x55) || (preamble[1]!=0xFF) )
		return WRONGFRAME;

	if (UARTRecieveMicro(FRAMEGAP,&header[0],6))
		return TIMEOUT;

	if (Crc8(&header[0],6))
		return WRONGFRAME;

	frameType=header[0];
	destinationAddress=header[1];
	sourceAddress=header[2];
	frameLength=(header[3]>>8)+header[4];

	if (UARTRecieveMicro(FRAMEGAP,&apdu[0],frameLength+2))
		return TIMEOUT;

	if (Crc16(&apdu[0],frameLength+2))
		return WRONGFRAME;

	return 0;
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	UCA0IE &= ~UCSTTIE;                         // Disable RX interrupt
	recieveFlag=1;
	__bic_SR_register_on_exit(LPM0_bits);
}
