#include <reg52.h>

struct timer
{
	unsigned char year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char week;
};
unsigned char flag200ms=1;
unsigned char setIndex=0;
unsigned char T0RL=0;
unsigned char T0RH=0;
struct timer *clock;

extern void InitLcd();
extern void InitDS1302();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void MoveM(unsigned char x,unsigned char y);
extern void LcdOpenCursor();
extern void LcdCloseCursor();
extern void KeyDriver();
extern void ReadTime(struct timer *clock);
extern void ChangeTime(struct timer *clock);
void ShowClock(struct timer *clock);
void ShowLight(unsigned char index);
void main()
{
	EA=1;
	InitLcd();
  InitDS1302();
	while(1)
	{
		if(flag200ms)
		{
			if(setIndex!=0)
			{
				ReadTime(clock);
			}
			else
			  ShowLight(setIndex);
			ShowClock(clock);
		}
		KeyDriver();
	}
}
void ShowLight(unsigned char index)
{
	switch(index)
	{
		case 1:MoveM(2,0);break;
		case 2:MoveM(3,0);break;
		case 3:MoveM(5,0);break;
		case 4:MoveM(6,0);break;
		case 5:MoveM(8,0);break;
		case 6:MoveM(9,0);break;
		case 7:MoveM(1,1);break;
		case 8:MoveM(2,1);break;
		case 9:MoveM(4,1);break;
		case 10:MoveM(5,1);break;
		case 11:MoveM(7,1);break;
		case 12:MoveM(8,1);break;
		default:break;
	}
}
unsigned char BCDHighup(unsigned char bcd)
{
	unsigned char bcdH=0;
	bcdH=(bcd>>4);
	if(bcdH<9)
	 bcdH++;
	bcd=bcdH+(bcd&0x0f);
}
unsigned char BCDHighdown(unsigned char bcd)
{
	unsigned char bcdH=0;
	bcdH=(bcd>>4);
	if(bcdH>0)
	 bcdH--;
	bcd=bcdH+(bcd&0x0f);
}
unsigned char BCDLowdown(unsigned char bcd)
{
	if(bcd&0x0f)
	 bcd=bcd-0x01;
	return bcd;
}
unsigned char BCDLowup(unsigned char bcd)
{
	if((bcd&0x0f)<9)
	 bcd=bcd+0x01;
	return bcd;
}
void ChangeClockup(struct timer *clock,unsigned char index)
{
	switch(index)
	{
		case 1:clock->year=BCDHighup(clock->year);break;
		case 2:clock->year=BCDLowup(clock->year);break;
		case 3:clock->mon=BCDHighup(clock->mon);break;
		case 4:clock->mon=BCDLowup(clock->mon);break;
		case 5:clock->day=BCDHighup(clock->day);break;
		case 6:clock->day=BCDLowup(clock->day);break;
		case 7:clock->hour=BCDHighup(clock->hour);break;
		case 8:clock->hour=BCDLowup(clock->hour);break;
		case 9:clock->min=BCDHighup(clock->min);break;
		case 10:clock->min=BCDLowup(clock->min);break;
		case 11:clock->sec=BCDHighup(clock->sec);break;
		case 12:clock->sec=BCDLowup(clock->sec);break;
		default:break;
	}
}
void ChangeClockdown(struct timer *clock,unsigned char index)
{
	switch(index)
	{
		case 1:clock->year=BCDHighdown(clock->year);break;
		case 2:clock->year=BCDLowdown(clock->year);break;
		case 3:clock->mon=BCDHighdown(clock->mon);break;
		case 4:clock->mon=BCDLowdown(clock->mon);break;
		case 5:clock->day=BCDHighdown(clock->day);break;
		case 6:clock->day=BCDLowdown(clock->day);break;
		case 7:clock->hour=BCDHighdown(clock->hour);break;
		case 8:clock->hour=BCDLowdown(clock->hour);break;
		case 9:clock->min=BCDHighdown(clock->min);break;
		case 10:clock->min=BCDLowdown(clock->min);break;
		case 11:clock->sec=BCDHighdown(clock->sec);break;
		case 12:clock->sec=BCDLowdown(clock->sec);break;
		default:break;
	}
}
void KeyAction(unsigned char keycode)
{
	if(keycode=='4')
	{
		if((setIndex!=0)&&(setIndex>12))
		{
			setIndex--;
		}
	}
	if(keycode=='6')
	{
		if((setIndex!=0)&&(setIndex<12))
		{
			setIndex++;
		}
	}
	if(keycode=='2')
	{
		if((setIndex!=0))
		{
			ChangeClockup(clock,setIndex);
		}
	}
	if(keycode=='8')
	{
		if((setIndex!=0))
		{
			ChangeClockdown(clock,setIndex);
		}
	}
	if(keycode=='5')
	{
		if(setIndex==0)
		{
			LcdOpenCursor();
			setIndex=1;
		}
		else
		{
			ChangeTime(clock);
			setIndex=0;
			LcdCloseCursor();
		}
	}
}
void ShowClock(struct timer *clock)
{
	unsigned char str_one[12];
	unsigned char str_two[12];
	
	str_one[0]='2';
	str_one[1]='0';
	str_one[3]=(((unsigned char)clock->year)>>4)+'0';
	str_one[4]=(((unsigned char)clock->year)&0x0f)+'0';
	str_one[5]=' ';
	str_one[6]=((clock->mon)>>4)+'0';
	str_one[7]=((clock->mon)&0x0f)+'0';
	str_one[8]=' ';
	str_one[9]=((clock->day)>>4)+'0';
	str_one[10]=((clock->day)&0x0f)+'0';
	str_one[11]='\0';
	
	str_two[0]=((clock->hour)>>4)+'0';
	str_two[1]=((clock->hour)&0x0f)+'0';
	str_two[2]=':';
	str_two[3]=((clock->min)>>4)+'0';
	str_two[4]=((clock->min)&0x0f)+'0';
	str_two[5]=':';
	str_two[6]=((clock->sec)>>4)+'0';
	str_two[7]=((clock->sec)&0x0f)+'0';
	str_two[8]='\0';
	
	LcdShowStr(0,0,str_one);
	LcdShowStr(0,0,str_two);
}
void ConfigTimer0(unsigned int ms)
{
	unsigned long tmp;
	tmp = 11059200 / 12; 
	tmp = (tmp * ms) / 1000; 
	tmp = 65536 - tmp; 
	tmp = tmp + 12; 
	T0RH = (unsigned char)(tmp>>8); 
	T0RL = (unsigned char)tmp; 
	TMOD &= 0xF0;  
	TMOD |= 0x01; 
	TH0 = T0RH; 
	TL0 = T0RL; 
	ET0 = 1; 
	TR0 = 1;
}

void InterruptTimer0() interrupt 1
{
	static unsigned char tmr200ms = 0;
	TH0 = T0RH; 
	TL0 = T0RL;
	tmr200ms++;
	if(tmr200ms>=200)
	{
		flag200ms=1;
		tmr200ms=0;
	}
}
