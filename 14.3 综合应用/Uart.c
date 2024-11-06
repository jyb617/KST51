#include <reg52.h>
 bit flagTxd=0;
 bit flagFrame=0;
 unsigned char cxdTxd=0;
 unsigned char pdata bufTxd[64];
 extern void UartAction(unsigned char *buf,unsigned char len);
 unsigned char UartRead(unsigned char *buf,unsigned char len)
 {
	 unsigned char i=0;
	 if(len>cxdTxd)
	 {
		 len=cxdTxd;
	 }
	 for(i=0;i<len;i++)
	 {
		 *buf++=bufTxd[i];
	 }
	 cxdTxd=0;
	 return len;
 }
 void UartMonitor(unsigned char ms)
 {
	 static unsigned char cxdPxd=0;
	 static unsigned char idms=0;
	 if(cxdTxd>0)
	 {
		 if(cxdPxd!=cxdTxd)
		 {
			 cxdPxd=cxdTxd;
		 }
		 else
		 {
			 if(idms<30)
			 {
				 idms+=ms;
				 if(idms>30)
				 {
					 flagFrame=1;
				 }
			 }
		 }
	 }
	 else
	 {
		 cxdPxd=0;
	 }
 }
 void UartDriver()
 {
	 unsigned char len=0;
	 unsigned char pdata buf[40];
	 if(flagFrame)
	 {
		 flagFrame=0;
		 len=UartRead(buf,sizeof(buf));
		 UartAction(buf,len);
	 }
 }
 
 void UartWrite(unsigned char *buf,unsigned len)
 {
	 while(len--)
	 {
		 flagTxd=0;
		 SBUF=*buf++;
		 while(!flagTxd);
	 }
 }
 void ConfigUart(unsigned int ms)
 {
	 SCON=0x50;
	 TMOD&=0x0F;
	 TMOD|=0x20;
	 TH1=256-(11059200/12/32)/ms;
	 TL1=TH1;
	 ET1=0;
	 ES=1;
	 TR1=1;
 }
 void InterruptTimer1() interrupt 4
 {
	 if(RI)
	 {
		 RI=0;
		 if(cxdTxd<sizeof(bufTxd))
		 {
		   bufTxd[cxdTxd++]=SBUF;
		 }
	 }
	 if(TI)
	 {
		 TI=0;
		 flagTxd=1;
	 }
 }