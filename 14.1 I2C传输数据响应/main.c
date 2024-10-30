#include <reg52.h>
#include <intrins.h>

#define I2CDelay() {_nop_();_nop_();_nop_();_nop_();}
sbit I2C_SCL=P3^7;
sbit I2C_SDA=P3^6;

extern void InitLcd1602();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
bit I2CAddressing(unsigned char addr);
void main()
{
	bit ack;
	unsigned char str[10];
	
	ack=I2CAddressing(0x50);
	str[0]='5';
	str[1]='0';
	str[2]=':';
	str[3]=(unsigned char)ack+'0';
	str[4]='0';
	InitLcd1602();
	LcdShowStr(0,0,str);
	
	ack=I2CAddressing(0x62);
	str[0]='6';
	str[1]='2';
	str[2]=':';
	str[3]=(unsigned char)ack+'0';
	str[4]='0';
	LcdShowStr(8,0,str);
	
	while(1);
}
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
 bit I2CWriting(unsigned char dat)
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
	
	return ack;
}
bit I2CAddressing(unsigned char addr)
{
	bit ack;
	I2CStart();
	ack=I2CWriting(addr<<1);
	I2CEnd();
	return ack;
}