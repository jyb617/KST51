#include <reg52.h>

bit statue=0;//change urcode or testing urcode
unsigned char urbuf[10];
unsigned char urcode[10];//urcode cache
unsigned char e2buf[10];
unsigned char T0RL;
unsigned char T0RH;
extern void E2PRead(unsigned char *buf,unsigned char addr,unsigned char len);
extern void E2PWrite(unsigned char *buf,unsigned char addr,unsigned char len);
extern void InitLcd();
extern void KeyScan();
extern void KeyDriver();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void LcdFullClear();
extern void ConfigUart(unsigned int baud);
void ConfigTimer0(unsigned int ms);
void main()
{
	EA=1;
	ConfigTimer0(1);
	ConfigUart(9600);
	InitLcd();
	LcdShowStr(0,0,"enter code");
	while(1)
	{
		KeyDriver();
	}
}
unsigned char CharToString(unsigned char dat, unsigned char *str)
{
	signed char i;
	unsigned char len=0;
	unsigned char buf[12];
	do{
		buf[i++]=dat%10;
		dat/=10;
	}while(dat>0);
	len+=i;
	while(i-->0)
	{
		*str++=buf[i]+'0';
	}
	*str='\0';
	return len;
}
bit Cmpcmd(unsigned char *ptr1,unsigned char *ptr2 ,unsigned char len)
{
	while(len--)
	{
		if(*ptr1++!=*ptr2++)
			return 0;
	}
	return 1;
}
void KeyAction(unsigned char keycode)
{
	static unsigned char addr=0x00;
	static unsigned char codelength0=0;
	unsigned char buf2[3];
	unsigned char buf1[3];
	if(statue==1)
	{
		if((keycode>='1')&&(keycode<='9'))
		{
			addr=addr&0x05;
			CharToString(keycode,buf2);
			LcdShowStr(addr,1,buf2);
			urcode[addr]=keycode;
			addr++;
		}
		if(keycode==0x1B)
		{
			statue=0;
			LcdFullClear();
			LcdShowStr(0,0,"change code");
			addr=0;
		}
		if(keycode==0x0d)
		{
			E2PWrite(urcode,0x00,6);
		}
	}
	else
	{
		if((keycode>='1')&&(keycode<='9'))
		{
			addr=addr&0x05;
			CharToString(keycode,buf1);
			LcdShowStr(addr,1,buf1);
			urbuf[addr]=keycode;
			addr++;
		}
		if(keycode==0x1B)
		{
			statue=1;
			LcdFullClear();
			LcdShowStr(0,0,"enter code");
			addr=0;
		}
		if(keycode==0x0d)
		{
			E2PRead(e2buf,0x00,6);
			if(Cmpcmd(e2buf,urbuf,6))
			{
				LcdShowStr(0,0,"yes");
			}
			else
			{
				LcdShowStr(0,0,"no");
			}
		}
	}
		
}
void ConfigTimer0(unsigned int ms)
{
	unsigned long tmp;
	
	tmp=11059200/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp;
	tmp=tmp+33;
	
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TMOD&=0xf0;
	TMOD|=0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0=1;
	TR0=1;
}
void InterruptTimer0() interrupt 1
{
	TH0 = T0RH;
	TL0 = T0RL;
	KeyScan();
}