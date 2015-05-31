/*
 * 18b20.c
 *
 *  Created on: 31 мая 2015 г.
 *      Author: landyrev
 */
#include <init.h>
#include <onewire.h>

unsigned short int temperatureRead()
{
	if (OWreset())
	{
		char c[2];
		OWputch(0xCC);
		OWputch(0x44);
		delay_micro(6000);
		OWreset();
		OWputch(0xCC);
		OWputch(0xBE);
		c[0]=OWreadch();
		c[1]=OWreadch();
		return (125/20)*(c[0]+(c[1]<<8));
	} else
		return 0;
}
