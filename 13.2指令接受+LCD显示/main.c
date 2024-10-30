#include <reg52.h>
sbit BUZZ=P1^6;
bit flagBuzz=0;
unsigned char T0RH=0;
unsigned char T0RL=0;
extern void UartMonitor(unsigned char ms);
extern void UartDriver();
extern void UartWrite(unsigned char *buf,unsigned len);
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void ConfigUart(unsigned int ms);
extern void LcdAreaClear(unsigned char x,unsigned char y,unsigned char len);
extern void InitLcd1602();
void ConfigTimer0(unsigned int ms);
void main()
{
	EA=1;
	ConfigTimer0(1);
	ConfigUart(9600);
	InitLcd1602();
	while(1)
	{
		UartDriver();
	}
}
void ConfigTimer0(unsigned int ms)
{
	unsigned long tmp;
	tmp=11059200/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp;
	tmp=tmp+33;
	TMOD&=0xF0;
	TMOD|=0x01;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
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
void UartAction(unsigned char *buf,unsigned char len)
{
	unsigned char i;
	unsigned char code cmd1[]="buzz on";
	unsigned char code cmd2[]="buzz off";
	unsigned char code cmd3[]="show str";
	unsigned char code cmdlen[]={sizeof(cmd1)-1,sizeof(cmd2)-1,sizeof(cmd3)-1};
	unsigned char code *cmdname[]={&cmd1[0],&cmd2[0],&cmd3[0]};
	for(i=0;i<sizeof(cmdlen);i++)
	{
		if(len>=cmdlen[i])
		{
			if(Cmpcmd(buf,cmdname[i],cmdlen[i]))
			{
				break;
			}
		}
	}
	switch(i)
	{
		case 0:flagBuzz=1;break;
		case 1:flagBuzz=0;break;
		case 2:
			buf[len]='\0';
		LcdShowStr(0,0,buf+cmdlen[2]);
		i=len-cmdlen[2];
		if(i<16)
		{
			LcdAreaClear(i,0,16-i);
		}
		break;
		default:UartWrite("bad command.\r\n", sizeof("bad command.\r\n")-1); return;
	}
	buf[len++]='\r';
	buf[len++]='\n';
	UartWrite(buf,len);
}
void InterruptTimer0() interrupt 1
{
	TH0=T0RH;
	TL0=T0RL;
	if(flagBuzz)
		BUZZ=~BUZZ;
	else
		BUZZ=1;
	UartMonitor(1);
}