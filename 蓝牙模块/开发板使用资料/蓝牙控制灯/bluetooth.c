#include <reg52.h> //ͷ�ļ����ã�д����ʱ��Ҫ����
#include <stdlib.h>
#define uint unsigned int //�궨�壬Ϊ�˺��涨�������д���
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
	SCON = 0x50; 	// SCON: ģʽ1, 8-bit UART, ʹ�ܽ���
	TMOD |= 0x20;
	TH1=0xfd; 		//������9600 ��ֵ
	TL1=0xfd;
	TR1= 1;
	EA= 1; 			//�����ж�
	ES= 1; 			//�򿪴����ж�
	while (1) 		//��ѭ�������κζ���
	{
	}
}
/******************************************************************/
/* �����жϳ���*/
/******************************************************************/
void UART_SER () interrupt 4
{
	uchar n; 	//������ʱ����
	if(RI) 		//�ж��ǽ����жϲ���
	{
		RI=0; 	//��־λ����
		n=SBUF; //���뻺������ֵ

		

		switch(n)
		{

			case 0X03:LED0=1;LED1=1;LED2=1;LED3=1;LED4=1;LED5=1;LED6=1;LED7=1;break;
			case 0X04:LED0=0;LED1=0;LED2=0;LED3=0;LED4=0;LED5=0;LED6=0;LED7=0;break;

		   }
	}

}