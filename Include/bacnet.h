/*
 * bacnet.h
 *
 *  Created on: 05 ???? 2015 ?.
 *      Author: landyrev
 */

#define WRONGFRAME 0x1
#define TIMEOUT 0x2
#define NO_TOKEN 0x3
#define USE_TOKEN 0x4

#define FRAMEGAP 2083
#define T_NO_TOKEN 500

//Master States
#define STATE_IDLE 1

unsigned char frameType;
unsigned char destinationAddress;
unsigned char sourceAddress;
unsigned short frameLength;
unsigned char apdu[501];
unsigned char apdu_out[501];
unsigned char npdu_out[510];
unsigned char recieveFlag;

int BACnetGetNPDU();
void DataNeedReply();
void DataNoReply();
void AnswerRequest(unsigned short ObjectId, unsigned short PropertyID, unsigned short InvokeId);
unsigned short EncodeACKReadPropertyAPDU(unsigned short InvokeId, unsigned short ObjectId, unsigned short PropertyID, unsigned short value);
unsigned short EncodeACKNPDU(unsigned short length);
unsigned short BACnetRoutine(unsigned short state);
