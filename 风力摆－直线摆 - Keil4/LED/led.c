#include "led.h"
#include "delay.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;				 //LED0-->PB.9 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.9
	GPIO_SetBits(GPIOE, GPIO_Pin_3);						 //PB.9 输出高
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
