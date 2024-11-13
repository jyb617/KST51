#include <reg52.h>

extern void I2CStart();
extern void I2CStop();
extern unsigned char I2CReadNAK();
extern unsigned char I2CReadACK();
extern bit I2CWrite(unsigned char dat);

void E2Read(unsigned char *buf, unsigned char addr,unsigned char len)
{
	do{
		I2CStart();
		if(I2CWrite(0x50<<1))
		{
			break;
		}
		I2CStop();
	}while(1);
	I2CWrite(addr);
	I2CStart();
	I2CWrite((0x50<<1)|0x01);
	while(len>1)
	{
		*buf++=I2CReadACK();
		len--;
	}
	*buf=I2CReadNAK();
	I2CStop();
}
void E2Write(unsigned char *buf, unsigned char addr, unsigned char len)
{
	while(len>0)
	{
		do{
			I2CStart();
			if(I2CWrite(0x50<<1))
				{
					break;
				}
				I2CStop();
		}while(1);
		I2CWrite(addr);
		while(len>0)
		{
			I2CWrite(*buf++);
			len--;
			addr++;
			if((addr&0x07)==0)
			{
				break;
			}
		}
	}
}
void MemToStr(unsigned char *str,unsigned char *src, unsigned char len)
{
	unsigned char tmp;
	
	while(len--)
	{
		tmp=*src>>4;
		if(tmp<=9)
			*str++=tmp+'0';
		else
			*str++=tmp-10+'A';
		tmp=*src&0x0f;
		if(tmp<=9)
			*str++=tmp+'0';
		else
			*str++=tmp-10+'A';
		*str++=' ';
		src++;
	}
}
