/**********************�ǲ�����С������ѭ������************************
*  ƽ̨��BST-V51  + Keil uVision3 + STC89C52RC 
*  ���ƣ�����С������
*  ��˾���������ǲ����ܿƼ����޹�˾     
*  ��д���޹�
*  ���ڣ�2016-5-10
*  ����:11.0592MHZ
*  ˵������ѿ�Դ�����ṩԴ���������������ֱ�ӵ�����Ⱥ����
*  Ӳ�����ã�Ҫ���Լ�����������������װ���ߺʹ����������ȵĵ��ԣ��������ʵ��
*  ʹ��˵������������IO���Լ��öŰ������Ӹ���ģ�飬�����Լ��޸ĸ���ģ��IO�� 
******************************************************************/
 //����K4������1����������С��
 //���¸�λ������ֹͣС��	

#include <reg52.h>	     //����52ϵͳͷ�ļ�
#include "bst_car.h"	 //����bst_car.h����С��ͷ�ļ�

unsigned char pwm_val_left  =0;//��������
unsigned char pwm_val_right =0;
unsigned char push_val_left ;// ����ռ�ձ�N/20 //�ٶȵ��ڱ��� 0-20������0��С��20���
unsigned char push_val_right ;// �ҵ��ռ�ձ�N/20 
							   // ��ҿ��Ը���ѭ��Ч���޸�����������ֵ 
bit Right_PWM_ON=1;	           //�ҵ��PWM����
bit Left_PWM_ON =1;			   //����PWM����

unsigned char control=0X03;//���˶�����ȫ�ֱ���

//��ʱ����	
void delay(unsigned int xms)				
{
    unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms����ʱԼxms����
    for(j=112;j>0;j--);
}

//ǰ��
 void run(void)
{
    //push_val_left=5;	 //�ٶȵ��ڱ��� 0-20������0��С��20���
	//push_val_right=5;
	Left_moto_go ;   //������ǰ��
	Right_moto_go ;  //�ҵ����ǰ��
}

//��ת
 void leftrun(void)
{	 
     //push_val_left=5;
	 //push_val_right=5;
	 Right_moto_go ;  //�ҵ����ǰ��
     Left_moto_Stop ;  //����ֹͣ
}

//��ת
void  rightrun(void)
{ 
	 //push_val_left=5;
	 //push_val_right=5;
     Left_moto_go  ;   //������ǰ��
	 Right_moto_Stop   ;  //�ҵ����ǰ��	
}

//����
void back(void)
{

   Left_moto_back  ;  // ���ֺ���
  Right_moto_back  ;  // ���ֺ���


}

//ͣת
void stop(void)
{
  Right_moto_Stop  ;   //ͣת
  Left_moto_Stop   ;   //

}


//����ת
void tleft(void)
{
  Left_moto_go  ;   //������ǰ��
  Right_moto_back  ;  // ���ֺ���
}

//����ת
void tright(void)
{
  Left_moto_back  ;   //������ǰ��
  Right_moto_go ;  // ���ֺ���
}




/************************************************************************/
//                   PWM���Ƶ��ת��                                   

//                   ��������                                        
/*����push_val_left��ֵ�ı���ת��,ռ�ձ�*/
void pwm_out_left_moto(void)
{  
     if(Left_PWM_ON)
     {
          if(pwm_val_left<=push_val_left)
	      {
	           EN1=1; 
	      }
	      else 
	      {
	           EN1=0;
          }
          if(pwm_val_left>=32)
	      pwm_val_left=0;
     }
     else    
     {
          EN1=0;   //��δ����PWM��EN1=0 ���� ֹͣ
     }
}
/******************************************************************/
/*                    �ҵ������                                  */  
void pwm_out_right_moto(void)
{ 
    if(Right_PWM_ON)
    { 
        if(pwm_val_right<=push_val_right)	//20ms�ڵ�ƽ�ź� 111 111 0000 0000 0000 00
	    {
	        EN2=1; 							//ռ�ձ�6:20
        }
	    else 
	    {
	        EN2=0;
        }
	    if(pwm_val_right>=32)
	    pwm_val_right=0;
    }
    else    
    {
        EN2=0;	  //��δ����PWM��EN2=0 �ҵ�� ֹͣ
    }
}
       
       
//TIMER0�жϷ����Ӻ�������PWM�ź�
void timer0()interrupt 1 using 2
{
     TH0=0XFC;	  //1Ms��ʱ
	 TL0=0X66;
	 pwm_val_left++;
	 pwm_val_right++;
	 pwm_out_left_moto();
	 pwm_out_right_moto();
 }	

 /******************************************************************/
/* �����жϳ���*/
/******************************************************************/
void UART_SER () interrupt 4
{
	unsigned char n; 	//������ʱ����

	if(RI) 		//�ж��ǽ����жϲ���
	{
		RI=0; 	//��־λ����
		n=SBUF; //���뻺������ֵ

		control=n;

		switch(n)
		{
			case 0X08:push_val_left =1;push_val_right =1;break;
			case 0X09:push_val_left =4;push_val_right =4;break;
			case 0X0A:push_val_left =8;push_val_right =8;break;
			case 0X0B:push_val_left =12;push_val_right =12;break;
			case 0X0C:push_val_left =16;push_val_right =16;break;
			case 0X0D:push_val_left =20;push_val_right =20;break;
			case 0X0E:push_val_left =24;push_val_right =24;break;
			case 0X0F:push_val_left =28;push_val_right =28;break;
		}
	}

}


 

void keyscan(void)              //����ɨ�躯��
{
    A:    if(K4==0)			//�ж��Ƿ��а����ź�
		{
		    delay(10);		  //��ʱ10ms
			if(K4==0)			//�ٴ��ж��Ƿ���
			 {
			    FM=0;               //��������  		
			    while(K4==0);	//�ж��Ƿ��ɿ�����
			    FM=1;               //������ֹͣ  
		 	 }
		    else
		     {
		       goto A;        //��ת��A���¼��
	              }
		}
		else
		{
		  goto A;             //��ת��A���¼��
		}
}


//pwm��ʼ��
void pwmint(void)
{
	TMOD=0X01;
	   TH0= 0XFC;  //1ms��ʱ
    TL0= 0X66;
	TR0= 1;
	ET0= 1;
	EA = 1;	    //�����ж�

}

//������ʼ��
void boothint(void)
{
   
   	SCON = 0x50; 	// SCON: ģʽ1, 8-bit UART, ʹ�ܽ���
	TMOD |= 0x20;
	TH1=0xfd; 		//������9600 ��ֵ
	TL1=0xfd;
	TR1= 1;
	EA = 1;	    //�����ж�
	ES= 1; 			//�򿪴����ж�


}


//������
void main(void)
{	
    P0=0X00;    //�ص��	
    keyscan();	//�����������
	delay(1000);//1s������

    pwmint();
	boothint();

	while(1)	//����ѭ��
	{ 

	   if(control>0X07)
	   {
	   
	   	stop();
	   
	   }



		switch(control)
		{
			case 0X01:run();break;
			case 0X02:leftrun();break;
			case 0X03:stop();break;
			case 0X04:rightrun();break;
			case 0X05:back();break;
			case 0X06:tleft();break;
			case 0X07:tright();break;
		   }

			 
	 }

}
   


