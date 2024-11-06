#include <reg52.h>
unsigned char T0RH=0;
unsigned char T0RL=0;

void InitShowStr();
void ConfigTimer0(unsigned int ms);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
extern void E2Read(unsigned char *buf, unsigned char addr, unsigned char len);
extern void E2Write(unsigned char *buf, unsigned char addr, unsigned char len);
extern void UartDriver();
extern void ConfigUART(unsigned int baud);
extern void UartRxMonitor(unsigned char ms);
extern void UartWrite(unsigned char *buf, unsigned char len);
void main()
{
	EA=1;
	ConfigTimer0(1);
	ConfigUART(9600);
	InitLcd1602();
	InitShowStr();
	
	while(1)
	{
		UartDriver();
	}
}
void InitShowStr()
{
	unsigned char str[17];
	
	str[16]='\0';
	E2Read(str,0x20,16);
	LcdShowStr(0,0,str);
	E2Read(str,0x40,16);
	LcdShowStr(0,1,str);
}
bit CmpMemory(unsigned char *ptr1,unsigned char *ptr2,unsigned char len)
{
	while(len--)
	{
		if(*ptr1++!=*ptr2++)
			return 0;
	}
	return 1;
}
void TrimString16(unsigned char *out, unsigned char *in)
{
	unsigned char i=0;
	
	while(*in !='\0')
	{
		*out++=*in++;
		i++;
		if(i>=16)
		{
			break;
		}
	}
	for(;i<16;i++)
	{
		*out++=' ';
	}
	*out='\0';
}

void UartAction(unsigned char *buf,unsigned char len)
{
	unsigned char i;
	unsigned char str[17];
	unsigned char code cmd0[]="showstr1";
	unsigned char code cmd1[]="showstr2";
	unsigned char code cmdLen[]={sizeof(cmd0)-1,sizeof(cmd1)-1};
	unsigned char code *cmdPtr[]={&cmd0[0],&cmd1[0],};
	
	for(i=0;i<sizeof(cmdLen);i++)
	{
		if(len>=cmdLen[i])
		{
			if(CmpMemory(buf,cmdPtr[i],cmdLen[i]))
				break;
		}
	}
	switch(i)
	{
		case 0:
			buf[len]='\0';
		TrimString16(str,buf+cmdLen[0]);
		LcdShowStr(0,0,str);
		E2Write(str,0x20,sizeof(str));
		break;
		case 1:
			buf[len]='\0';
		TrimString16(str,buf+cmdLen[0]);
		LcdShowStr(0,0,str);
		E2Write(str,0x40,sizeof(str));
		break;
		default:UartWrite("bad command.\r\n", sizeof("bad command.\r\n")-1); return;
	}
	buf[len++]='\r';
	buf[len++]='\n';
	UartWrite(buf,len);
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
	TH0=T0RH;
	TL0=T0RL;
	UartRxMonitor(1);
}