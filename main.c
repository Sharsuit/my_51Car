#include <REGX52.H>
#include "Delay.h"
//#include "LCD1602.h"
#include "Timer0.h"
#include "UART.h"
//sbit Motor=P1^0;
sbit Motor1=P1^0;
sbit Motor2=P1^1;
sbit MR=P0^0;
sbit NR=P0^1;
sbit ML=P0^2;
sbit NL=P0^3;
//循迹
int dect=0;
sbit L2=P1^2; 
sbit L1=P1^3; 
sbit C0=P1^4; 
sbit R1=P1^5; 
sbit R2=P1^6; 

sbit Led1=P0^4;
sbit Led2=P2^0;
sbit Led3=P2^1;
sbit Led4=P2^2;
sbit Swi =P0^5;
sbit Beez = P0^6;
unsigned char Sp1=25,Sp2=30    ,Sp3=40,Sp4=45;
unsigned char ctrl[3];
//电容赛道,停止线,横线转向引导
int num=0;
//unsigned char leader[]={'0','W','W','P'};//第一位备用
//unsigned char leader[]={'0','W','W','W','W','P'};//第一位备用
unsigned char leader[]={'0','W','W','W','A','W','P'};

                                          
unsigned char Counter,Compare,Compare1,Compare2;	//??????????????????PWM
unsigned char KeyNum,Speed;
int previous=4,current=4;
void saidao(char a)
{
	if(a==1)
	{
		num=0;
		leader[0]='0';
		leader[1]='W';
		leader[2]='W';
		leader[3]='P';
		leader[4]='0';
		leader[5]='0';
		leader[6]='0';
		Led2=1;
		Led3=1;
		Led4=1;
		Led2=0;
		Beez=0;Delay(100);Beez=1;
		
	}
	else if(a==2)
	{
		num=0;
		leader[0]='0';
		leader[1]='W';
		leader[2]='W';
		leader[3]='W';
		leader[4]='W';
		leader[5]='P';
		leader[6]='0';
		Led2=1;
		Led3=1;
		Led4=1;
		Led2=0;
		Led3=0;
		Beez=0;Delay(100);Beez=1;
	}
	else if(a==3)
	{
		num=0;
		leader[0]='0';
		leader[1]='W';
		leader[2]='W';
		leader[3]='W';
		leader[4]='A';
		leader[5]='W';
		leader[6]='P';
		Led2=1;
		Led3=1;
		Led4=1;
		Led2=0;
		Led3=0;
		Led4=0;
		Beez=0;Delay(100);Beez=1;
	}
}
int Changer()
{
	previous=current;
	current=update();
	if(current==previous)
	{
		return 1;
	}
	return 0;
}
void changeSpeedLoop()//
{
	if(P3_4==0)
	{
	Compare=75;P2_2=0;
	}
	else if(P3_2==0)
	{
	P2_2=0;Compare=25;
	}
	else if(P3_3==0)
	{
	P2_2=0;Compare=50;
	}
	else if(P3_5==0)
	{
	P2_2=0;Compare=100;
	}
	else
	{
	P2_2=1;
	}
}

void FORMAL_RUN()
{
	if(ctrl[1]=='W')
	{
	ML=0;
	NL=1;
	MR=0;
	NR=1;
	}
	else if(ctrl[1]=='A')
	{
	ML=1;
	NL=0;
	MR=0;
	NR=1;
	}else if(ctrl[1]=='S')
	{
	ML=1;
	NL=0;
	MR=1;
	NR=0;
	}else if(ctrl[1]=='D')
	{
	ML=0;
	NL=1;
	MR=1;
	NR=0;
	}else if(ctrl[1]=='a')
	{
	ML=0;
	NL=0;
	MR=0;
	NR=1;
	}else if(ctrl[1]=='d')
	{
	ML=0;
	NL=1;
	MR=0;
	NR=0;
	}		
	else
	{
	MR=0;
	NR=0;
	ML=0;
	NL=0;	
	}
}


void AutoRun(char txxt)
{
	
	switch(txxt)
		{
		case'W':ctrl[1]='W';FORMAL_RUN();break;
		case'w':ctrl[1]='W';FORMAL_RUN();break;
		case'A':ctrl[1]='A';FORMAL_RUN();break;
		case'Q':ctrl[1]='W';FORMAL_RUN();break;
		case'S':ctrl[1]='S';FORMAL_RUN();break;
		case'D':ctrl[1]='D';FORMAL_RUN();break;
		case'E':ctrl[1]='W';FORMAL_RUN();break;
		case'a':ctrl[1]='a';FORMAL_RUN();break;
		case'd':ctrl[1]='d';FORMAL_RUN();break;
		case'P':ctrl[1]='P';FORMAL_RUN();break;
		case'1':Compare1=10;Compare2=10;break;
		case'2':Compare1=20;Compare2=20;break;
		case'3':Compare1=30;Compare2=30;break;
		case'4':Compare1=40;Compare2=40;break;
		case'5':Compare1=50;Compare2=50;break;
		case'6':Compare1=60;Compare2=60;break;
		case'7':Compare1=70;Compare2=70;;break;
		case'8':Compare1=80;Compare2=80;break;
		}
	
}


int update()
{
	L2?(dect=dect|0x10):(dect=dect&0x0F);
	L1?(dect=dect|0x08):(dect=dect&0x17);
	C0?(dect=dect|0x04):(dect=dect&0x1B);
	R1?(dect=dect|0x02):(dect=dect&0x1D);
	R2?(dect=dect|0x01):(dect=dect&0x1E);
	0?(dect=dect|0x00):(dect=dect&0xFF);
	return dect;
}

void doLine()
{
	if(leader[num]!='P')
	{
	num+=1;
	}
	switch(leader[num])
	{
		case'W':Compare1=Sp1;Compare2=Sp1;AutoRun('W');Beez=0;Led1=0;update();while(!(update()==0x1F||update()==0x1B));break;//---
		case'D':Compare1=Sp1;Compare2=Sp1;AutoRun('D');Beez=0;Led1=0;Delay(1000);while(update()!=0x1B);break;
		case'A':Compare1=Sp1;Compare2=Sp1;AutoRun('A');Beez=0;Led1=0;Delay(1000);while(update()!=0x1B);break;
		case'P':AutoRun('P');Led1=0;Beez=1;Delay(100);Beez=0;while(1);break;
	}
	
}

void xunjiLoop()
{
	unsigned char zhuan;
	int temp,i;
	if(ctrl[0])
	{
	update();
	if(dect==0x1B){Compare1=Sp1;Compare2=Sp1;AutoRun('W');Beez=1;Led1=1;}//11011
	if(dect==0x00){AutoRun('P');Delay(1000);doLine();}      //00000     //    sheng guang ti shi  doLine();
	if(dect==0x1F){Beez=1;Led1=1;}//11111
	if(dect==0x17){Compare1=Sp1;Compare2=Sp2;AutoRun('Q');}//10111
	if(dect==0x1D){Compare1=Sp2;Compare2=Sp1;AutoRun('E');}//11101
	if(dect==0x18)//11000
	{
		temp=0x1f;
		for(i=0;i<25;i++)
		{
			temp&=update();
			Delay(1);
		}
		if(temp==0x00)
		{
			doLine();
		}
		else if(temp==0x18)
		{
		Compare1=Sp1;Compare2=Sp1;AutoRun('D');
		while(update()!=0x1B)//11011
		{
			if(dect==0x00) break;
		}
		}
		
	}
	if(dect==0x03)//00011
	{
		temp=0x1f;
		for(i=0;i<25;i++)
		{
			temp&=update();
			Delay(1);
		}
		if(temp==0x00)
		{
			doLine();
		}
		else if(temp==0x03)
		{
		//while()
		Compare1=Sp1;Compare2=Sp1;AutoRun('A');
		while(update()!=0x1B)
		{
			if(dect==0x00) break;
		}
		}
			
		}
		
	}
	/*
	if(dect==0x1E)//11110
	{
		while(!(R2==0||update()==0x1f));
		if(dect==0x1f)
		{
			Compare1=30;Compare2=30;AutoRun('D');
			while(update()!=0x1B)
			{
				if(dect==0x00) break;
			}
		}
	}
	*/
	
	
	if(dect==0x1C)//11100
	{
		temp=0x1f;
		for(i=0;i<25;i++)
		{
			temp&=update();
			Delay(1);
		}
		if(temp==0x00)
		{
			doLine();
		}
		else if(temp==0x1C||temp==0x18)
		{
		if((1))
		{
		Compare1=Sp1;Compare2=Sp1;AutoRun('D');
		while(update()!=0x1B)
		{
			if(dect==0x00) break;
		}
		}
			
		}
	}
	
	/*//影响电容赛道 待改善
	if(dect==0x0F)//01111
	{
		while(!(L2==0||update()==0x1f));
		if(dect==0x1f)
		{
			Compare1=30;Compare2=30;AutoRun('A');
			while(update()!=0x1B)
			{
				if(dect==0x00) break;
			}
		}
	}
	*/
	
	if(dect==0x07)//00111
	{
		temp=0x1f;
		for(i=0;i<25;i++)
		{
			temp&=update();
			Delay(1);
		}
		if(temp==0x00)
		{
			doLine();
		}
		else if(temp==0x03||temp==0x07)
		{
		if((1))
		{
		Compare1=Sp1;Compare2=Sp1;AutoRun('A');
		while(update()!=0x1B)
		{
			if(dect==0x00) break;
		}
		}
			
		}
	}
}
void doBlueTooth(char txt)
{
	switch(txt)
		{
		case'W':ctrl[1]='W';FORMAL_RUN();break;
		case'A':ctrl[1]='A';FORMAL_RUN();break;
		case'S':ctrl[1]='S';FORMAL_RUN();break;
		case'D':ctrl[1]='D';FORMAL_RUN();break;
		case'a':ctrl[1]='a';FORMAL_RUN();break;
		case'd':ctrl[1]='d';FORMAL_RUN();break;
		case'P':ctrl[1]='P';FORMAL_RUN();break;
		case'1':Compare1=25;Compare2=25;break;
		case'2':Compare1=50;Compare2=50;break;
		case'3':Compare1=75;Compare2=75;break;
		case'4':Compare1=100;Compare2=100;break;
		case'5':break;
		case'6':break;
		case'7':break;
		case'8':break;
		case'U':ctrl[0]=1;AutoRun('P');break;
		case'R':num=0;break;
		case'J':saidao(1);break;
		case'K':saidao(2);break;
		case'L':saidao(3);break;
		case'I':AutoRun('P');ctrl[0]=0;AutoRun('P');ctrl[0]=0;ctrl[1]='P';break;//有bug 需要发两次才能正常触发 目前未能解决
		}
}
void main()
{
		Led2=1;
		Led3=1;
		Led4=1;
		Led2=0;
		Led3=0;
		Led4=0;  
	ctrl[0]=1;
	UART_Init();
	Timer0_Init();
	while(1)
	{
		      
		xunjiLoop();
		//changeSpeedLoop();
		FORMAL_RUN();
	}
}

void Timer0_Routine() interrupt 1
{
	TL0 = 0x9C;		
	TH0 = 0xFF;	
	//TL0 = 0x33;		
	//TH0 = 0x53;		
	Counter++;
	Counter%=100;
	if(Counter<Compare1)	
	{
		Motor1=1;		
	}
	else				
	{
		Motor1=0;		
	}
	if(Counter<Compare2)	
	{	
		Motor2=1;
	}
	else				
	{		
		Motor2=0;
	}
}

void UART_Routine() interrupt 4  
{
	if(RI==1)				
	{
		RI=0;
		ctrl[1]=SBUF;
		FORMAL_RUN();
		doBlueTooth(SBUF);
	    UART_SendByte(SBUF);	
	}
}

