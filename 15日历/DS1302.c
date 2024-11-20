#include <reg52.h>

sbit DS1302_CE=P1^7;
sbit DS1302_CK=P3^5;
sbit DS1302_IO=P3^4;

struct timer
{
	unsigned int year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char week;
};

void DS1302Write(unsigned char dat)
{
	unsigned char mask;
	
	for(mask=0x01;mask!=0;mask<<=1)
	{
		if((mask&dat)!=0)
			DS1302_IO=1;
		else
			DS1302_IO=0;
		DS1302_CK=1;
		DS1302_CK=0;
	}
	
	DS1302_IO=1;
}
void WriteDat(unsigned char addr,unsigned dat)
{
	DS1302_CE=1;
	DS1302Write((addr<<1)|0x80);
	DS1302Write(dat);
	DS1302_CE=0;
}
unsigned char DS1302Read()
{
	unsigned char mask;
	unsigned char dat=0;
	for(mask=0x01;mask!=0;mask<<=1)
	{
		if(DS1302_IO!=0)
			dat|=mask;
		DS1302_CK=1;
		DS1302_CK=0;
	}
	
	return dat;
}
unsigned char ReadDat(unsigned char addr)
{
	unsigned char dat;
	DS1302_CE=1;
	DS1302Write((addr<<1)|0x81);
	dat=DS1302Read();
	DS1302_CE=0;
	return dat;
}
void DS1302BurstRead(unsigned char *dat)
{
	unsigned char i;
	
	DS1302_CE=1;
	DS1302Write(0xBF);
	for(i=0;i<8;i++)
	{
		dat[i]=DS1302Read();
	}
	DS1302_CE=0;
}
void DS1302BurstWrite(unsigned char *dat)
{
	unsigned char i;
	
	DS1302_CE=1;
	DS1302Write(0xBE);
	for(i=0;i<8;i++)
	{
		DS1302Write(dat[i]);
	}
	DS1302_CE=0;
}
void InitDS1302()
{
	unsigned char addr;
	unsigned char dat;
	unsigned char i;
	unsigned char code time[] = { 0x00,0x30,0x12, 0x08, 0x10, 0x02, 0x13 };
	DS1302_CE=0;
	DS1302_CK=0;
	addr=0x00;
	dat=ReadDat(addr);
	if((dat&0x80)!=0)
	{
		WriteDat(7,0x00);
		for(i=0;i<7;i++)
		{
			WriteDat(i,time[i]);
		}
	}
}

void ChangeTime(struct timer *clock)
{
	unsigned char buf[8];
	
	buf[7]=0;
	buf[6]=clock->year;
	buf[5]=clock->week;
	buf[4]=clock->mon;
	buf[3]=clock->day;
	buf[2]=clock->hour;
	buf[1]=clock->min;
	buf[0]=clock->sec;
	
	DS1302BurstWrite(buf);
}

void ReadTime(struct timer *clock)
{
	unsigned char buf[8];
	
	DS1302BurstRead(buf);
	
  clock->year=buf[6];
	clock->week=buf[5];
	clock->mon=buf[4];
	clock->day=buf[3];
	clock->hour=buf[2];
	clock->min=buf[1];
	clock->sec=buf[0];
}