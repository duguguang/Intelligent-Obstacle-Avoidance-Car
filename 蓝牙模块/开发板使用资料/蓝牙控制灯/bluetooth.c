#include <reg52.h> //头文件调用，写程序时都要加上
#include <stdlib.h>
#define uint unsigned int //宏定义，为了后面定义变量书写简便
#define uchar unsigned char



sbit LED0=P1^0;
sbit LED1=P1^1;
sbit LED2=P1^2;
sbit LED3=P1^3;
sbit LED4=P1^4;
sbit LED5=P1^5;
sbit LED6=P1^6;
sbit LED7=P1^7;

void main (void)
{
	SCON = 0x50; 	// SCON: 模式1, 8-bit UART, 使能接收
	TMOD |= 0x20;
	TH1=0xfd; 		//波特率9600 初值
	TL1=0xfd;
	TR1= 1;
	EA= 1; 			//打开总中断
	ES= 1; 			//打开串口中断
	while (1) 		//主循环不做任何动作
	{
	}
}
/******************************************************************/
/* 串口中断程序*/
/******************************************************************/
void UART_SER () interrupt 4
{
	uchar n; 	//定义临时变量
	if(RI) 		//判断是接收中断产生
	{
		RI=0; 	//标志位清零
		n=SBUF; //读入缓冲区的值

		

		switch(n)
		{

			case 0X03:LED0=1;LED1=1;LED2=1;LED3=1;LED4=1;LED5=1;LED6=1;LED7=1;break;
			case 0X04:LED0=0;LED1=0;LED2=0;LED3=0;LED4=0;LED5=0;LED6=0;LED7=0;break;

		   }
	}

}