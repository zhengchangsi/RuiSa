#include "led.h"
#include "delay.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;				 //LED0-->PB.9 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.9
	GPIO_SetBits(GPIOE, GPIO_Pin_3);						 //PB.9 �����
}

void LED_OFF(void)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
}

void LED_ON(void)
{
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
}

void Beep_OFF(void)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_2);
}

void Beep_ON(void)
{
	GPIO_SetBits(GPIOE,GPIO_Pin_2);
}

void Beep_warning(void)
{
	Beep_ON();
	delay_ms(800);
	Beep_OFF();
	delay_ms(800);
}
