#include <reg52.h>
#define Lcd_Sta P0
sbit Lcd_RS=P1^0;
sbit Lcd_WR=P1^1;
sbit Lcd_E=P1^5;

void LcdReady()
{
	unsigned char sta;
	Lcd_RS=0;
	Lcd_WR=1;
	do{
	Lcd_E=1;
	sta=Lcd_Sta;
	Lcd_E=0;
	}while(sta&0x80);
}

void LcdWriteDat(unsigned char dat)
{
	LcdReady();
	Lcd_RS=1;
	Lcd_WR=0;
	Lcd_Sta=dat;
	Lcd_E=1;
	Lcd_E=0;
}

void LcdWriteCmd(unsigned char cmd)
{
	LcdReady();
	Lcd_RS=0;
	Lcd_WR=0;
	Lcd_Sta=cmd;
	Lcd_E=1;
	Lcd_E=0;
}
void MoveM(unsigned char x,unsigned char y)
{
	unsigned char addr;
	if(y==0)
	{
		addr=0x00+x;
	}
	else if(y==1)
	{
		addr=0x40+x;
	}
	LcdWriteCmd(addr|0x80);
}
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str)
{
	MoveM(x,y);
	while(*str!='\0')
	{
		LcdWriteDat(*str++);
	}
}

void LcdClear(unsigned char x,unsigned char y,unsigned char len)
{
	MoveM(x,y);
	while(len--)
	{
		LcdWriteDat(' ');
	}
}
void LcdFullClear()
{
	LcdWriteCmd(0x01);
}
void InitLcd()
{
	LcdWriteCmd(0x38);
	LcdWriteCmd(0x0c);
	LcdWriteCmd(0x06);
	LcdWriteCmd(0x01);
}