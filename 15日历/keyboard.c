#include <reg52.h>

sbit KeyIn1=P2^4;
sbit KeyIn2=P2^5;
sbit KeyIn3=P2^6;
sbit KeyIn4=P2^7;
sbit KeyOut1=P2^3;
sbit KeyOut2=P2^2;
sbit KeyOut3=P2^1;
sbit KeyOut4=P2^0;

extern void KeyAction(unsigned char i);

unsigned char code KeyCodeMap[4][4] = {
	{ '1', '2', '3', 0x26 },
	{ '4', '5', '6', 0x25 },
	{ '7', '8', '9', 0x28 },
	{ '0', 0x1B, 0x0D, 0x27 }
};

unsigned char pdata KeySta[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};

void KeyDriver()
{
	unsigned char i=0;
	unsigned char j=0;
	static unsigned char keybuf[4][4]={{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(KeySta[i][j]!=keybuf[i][j])
			{
				if(keybuf[i][j]!=0)
				{
					KeyAction(KeyCodeMap[i][j]);
				}
				keybuf[i][j]=KeySta[i][j];
			}
		}
	}
}
void KeyScan()
{
	static unsigned char keyout=0;
	static unsigned char KeyCode[4][4]={{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff}};
	unsigned char i;
	KeyCode[keyout][0]=(KeyCode[keyout][0]<<1)|KeyIn1;
	KeyCode[keyout][1]=(KeyCode[keyout][1]<<1)|KeyIn2;
	KeyCode[keyout][2]=(KeyCode[keyout][2]<<1)|KeyIn3;
	KeyCode[keyout][3]=(KeyCode[keyout][3]<<1)|KeyIn4;
	
	for(i=0;i<4;i++)
	{
		if((KeyCode[keyout][i]&0x0f)==0x00)
			KeySta[keyout][i]=0;
		else if((KeyCode[keyout][i]&0x0f)==0x0F)
			KeySta[keyout][i]=1;
	}
		
	keyout++;
	keyout&=0x03;
	switch(keyout)
	{
		case 0:KeyOut4=1;KeyOut1=0;break;
		case 1:KeyOut1=1;KeyOut2=0;break;
		case 2:KeyOut2=1;KeyOut3=0;break;
		case 3:KeyOut3=1;KeyOut4=0;break;
		default:break;
	}
}