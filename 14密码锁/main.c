#include <reg52.h>

unsigned char statue=0x00;//change urcode or testing urcode
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
extern void LcdShowStr1(unsigned char x,unsigned char y,unsigned char *str,unsigned char len);
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void LcdFullClear();
void ConfigTimer0(unsigned int ms);
void main()
{
	EA=1;
	ConfigTimer0(1);
	InitLcd();
	LcdShowStr(0,0,"enter code");
	while(1)
	{
		KeyDriver();
	}
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
	if(statue==0x01)
	{
		if((keycode>='1')&&(keycode<='9'))
		{
			if(addr>0x05)
			{
				addr=0x00;
			}
			urbuf[addr]=keycode;
			addr++;
			LcdShowStr1(0,1,urbuf,6);
		}
		else if(keycode==0x1B)
		{
			statue=0x00;
			LcdFullClear();
			LcdShowStr(0,0,"enter code");
			addr=0x00;
		}
		else if(keycode==0x0d)
		{
			E2PWrite(urbuf,0x20,0x06);
		}
	}
	else if(statue==0)
	{
		if((keycode>='1')&&(keycode<='9'))
		{
			if(addr>0x05)
			{
				addr=0;
			}
			urcode[addr]=keycode;
			addr++;
			LcdShowStr1(0,1,urcode,6);
		}
		else if(keycode==0x1B)
		{
			statue=0x01;
			LcdFullClear();
			LcdShowStr(0,0,"change code");
			addr=0x00;
		}
		else if(keycode==0x0d)
		{
			E2PRead(e2buf,0x20,6);
			if(Cmpcmd(e2buf,urcode,6))
			{
				LcdShowStr(0,0,"yes");
			}
			else
			{
				LcdShowStr1(0,0,"no");
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