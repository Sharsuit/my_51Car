#include <REGX52.H>

/**
  * @brief  串口初始化，4800bps@12.000MHz
  * @param  无
  * @retval 无
  */
void UART_Init()
{
	/*
	SCON=0x50; 
	PCON |= 0x80; //1000 0000  波特率加倍
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式 
	TL1 = 0xF3;		//设定定时初值   应为0xFA      
	TH1 = 0xF3;		//设定定时器重装值    
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
	*/
	SCON =0x50;
	PCON &= 0x7F;  //0111 1111 波特率不加倍
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
	//感谢电协学长提供代码参考!!!!
	//11.0592  串口1  9600  8位数据  不加倍 
	// 定时器1 8位自动重载  12T
}

/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}

/*串口中断函数模板
void UART_Routine() interrupt 4
{
	if(RI==1)
	{
		
		RI=0;
	}
}
*/
