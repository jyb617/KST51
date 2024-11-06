#include <reg52.h>
#include <intrins.h>

#define I2CDelay() {_nop_();_nop_();_nop_();_nop_();}
sbit I2C_SCL=P3^7;
sbit I2C_SDA=P3^6;

void I2CStart()
{
	I2C_SCL=1;
	I2C_SDA=1;
	I2CDelay();
	I2C_SDA=0;
	I2CDelay();
	I2C_SCL=0;
}
void I2CEnd()
{
	I2C_SCL=0;
	I2C_SDA=0;
	I2CDelay();
	I2C_SCL=1;
	I2CDelay();
	I2C_SDA=1;
	I2CDelay();
}
 bit I2CWrite(unsigned char dat)
{
	bit ack;
	unsigned char mate;
	for(mate=0x80;mate!=0;mate>>=1)
	{
		if((mate&dat)==0)
		{
			I2C_SDA=0;
		}
		else
			I2C_SDA=1;
		I2CDelay();
		I2C_SCL=1;
		I2CDelay();
		I2C_SCL=0;
	}
	I2C_SDA=1;
	I2CDelay();
	I2C_SCL=1;
	ack=I2C_SDA;
	I2CDelay();
	I2C_SCL=0;
	
	return (~ack);
}

unsigned char I2CReadNAK()
{
	unsigned char mask;
	unsigned char dat;
	
	I2C_SDA=1;
	for(mask=0x80;mask!=0;mask>>=1)
	{
		I2CDelay();
		I2C_SCL=1;
		if(I2C_SDA==0)
			dat&=~mask;
		else
			dat|=mask;
		I2CDelay();
		I2C_SCL=0;
	}
	I2C_SDA=1;
	I2CDelay();
	I2C_SCL=1;
	I2CDelay();
	I2C_SCL=0;
	
	return dat;
}
unsigned char I2CReadACK()
{
	unsigned char mask;
	unsigned char dat;
	
	I2C_SDA=1;
	for(mask=0x80;mask!=0;mask>>=1)
	{
		I2CDelay();
		I2C_SCL=1;
		if(I2C_SDA==0)
			dat&=~mask;
		else
			dat|=mask;
		I2CDelay();
		I2C_SCL=0;
	}
	I2C_SDA=0;
	I2CDelay();
	I2C_SCL=1;
	I2CDelay();
	I2C_SCL=0;
	
	return dat;
}
