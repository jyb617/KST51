#include <reg52.h>
unsigned char step=0;
unsigned char oprt=0;
signed long num1=0;
signed long num2=0;
signed long result=0;
unsigned char T0RH=0;
unsigned char T0RL=0;
extern void KeyDriver();
extern void KeyScan();
extern void InitLcd1602();
extern void LcdFullClear();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void LcdAreaClear(unsigned char x,unsigned char y,unsigned char len);
void ConfigTimer0(unsigned int ms);
void main()
{
	EA=1;
	ConfigTimer0(1);
	InitLcd1602();
	LcdShowStr(15, 1, "0");
	while(1)
	{
		KeyDriver();
	}
}
void AllClear()
{
	num1=0;
	num2=0;
	step=0;
	LcdFullClear();
}
unsigned char LongToString(signed long dat, unsigned char *str)
{
	signed char i;
	unsigned char len=0;
	unsigned char buf[12];
	if(dat<0)
	{
		dat=-dat;
		*str++='-';
		len++;
	}
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
void NumberAction(unsigned char n)
{
	unsigned char str[12];
	unsigned char len;
	if(step>1)
	{
		AllClear();
	}
	if(step==0)
	{
		num1=num1*10+n;
		len=LongToString(num1,str);
		LcdShowStr(16-len,1,str);
	}
	else if(step==1)
	{
		num2=num2*10+n;
		len=LongToString(num2,str);
		LcdShowStr(16-len,1,str);
	}
}
void OprtAction(unsigned char type)
{
	unsigned char str[12];
	unsigned char len;
	if(step==0)
	{
		len=LongToString(num1,str);
		LcdAreaClear(0,0,16-len);
		LcdShowStr(16-len,0,str);
		switch(type)
		{
			case 0:LcdShowStr(0,1,'+');break;
			case 1:LcdShowStr(0,1,'-');break;
			case 2:LcdShowStr(0,1,'*');break;
			case 3:LcdShowStr(0,1,'/');break;
		}
		LcdAreaClear(0,1,14);
		LcdShowStr(15, 1, "0");
		oprt=type;
		step=1;
	}
}
void GetResult()
{
	unsigned char len=0;
	unsigned char str[12];
	if(step==1)
	{
		step=2;
		switch(oprt)
		{
			case 0:result=num1+num2;break;
			case 1:result=num1-num2;break;
			case 2:result=num1*num2;break;
			case 3:result=num1/num2;break;
		}
		len=sizeof("result");
		LcdAreaClear(0,0,16-len);
		LcdShowStr(16-len,0,"result");
		len=LongToString(result,str);
		LcdAreaClear(1,1,16-len-1);
		LcdShowStr(0,1,'=');
		LcdShowStr(16-len,1,str);
	}
}
void KeyAction(unsigned char keycode)
{
	if((keycode>='0')&&(keycode<='9'))
	{
		NumberAction(keycode-'0');
	}
	else if(keycode==0x26)
	{
		OprtAction(0);
	}
	else if(keycode==0x28)
	{
		OprtAction(1);
	}
	else if(keycode==0x25)
	{
		OprtAction(2);
	}
	else if(keycode==0x27)
	{
		OprtAction(3);
	}
	else if(keycode==0x0d)
	{
		GetResult();
	}
	else if(keycode==0x1b)
	{
		AllClear();
		LcdShowStr(15, 1, "0");
	}
}
void ConfigTimer0(unsigned int ms)
{
	unsigned long tmp;
	tmp=1105920/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp;
	tmp=tmp+28;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TMOD&=0xF0;
	TMOD|=0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}
void InterruptTimer0() interrupt 1
{
	TH0=T0RH;
	TL0=T0RL;
	KeyScan();
}