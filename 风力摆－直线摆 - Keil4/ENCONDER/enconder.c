#include <stm32f10x.h>
#include "enconder.h"

s32 volatile G_Enconder_count1 = 0;
s32 volatile G_Enconder_count2 = 0;

s32 G_Goal_1 = 0;
s32 G_Goal_2 = 0;

u16   lastCount1 = 0;
u16   lastCount2 = 0;


void Enconder_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE); 	   
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2, ENABLE);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;						//PC6.7用为TIM3的通道1.2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);			// TIM3完全复用功能开启	
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;						//PA0.1用为TIM2的通道1.2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Enconder_init(void)
{
//	RCC->APB2ENR |= (1 << 2) | (1 << 0);	//开启GPIOA和AFIO时钟
	Enconder_IO_Init();
	
	TIM2->CCMR1 = 0x101;	//设置T1=CH1 T2=CH2，输入模式 
	TIM2->CCER = 0x00;	//设置极性为正
	TIM2->SMCR = 0x03;	//设置为从模式，编码器3模式

	TIM2->ARR = 0xffff;	//设置为最大，避免频繁重装初值
	TIM2->PSC = 0x00;	//设置分频为0
	
	TIM2->DIER |= 1<<0;
	TIM2->DIER |= 1<<6;
	TIM2->CR1	|= 0X01;

	TIM2->CR1 |= 1 << 7;	//开启影子寄存器
	TIM2->CR1 |= 1 << 0;	//开启定时器
	
	
	TIM3->CCMR1 = 0x101;	//设置T1=CH1 T2=CH2，输入模式 
	TIM3->CCER = 0x00;	//设置极性为正
	TIM3->SMCR = 0x03;	//设置为从模式，编码器3模式

	TIM3->ARR = 0xffff;	//设置为最大，避免频繁重装初值
	TIM3->PSC = 0x00;	//设置分频为0
	
	TIM3->DIER |= 1<<0;
	TIM3->DIER |= 1<<6;
	TIM3->CR1	|= 0X01;
	
	
	TIM3->CR1 |= 1 << 7;	//开启影子寄存器
	TIM3->CR1 |= 1 << 0;	//开启定时器
}

s16 ENC1_Get_Electrical_Angle(void)
{	
//	static u16  lastCount1 = 0;
	u16 curCount = TIM2->CNT;
	s32 dAngle = curCount - lastCount1;

	if(dAngle >= MAX_COUNT){
			dAngle -= ENCODER_TIM_PERIOD;
	}else if(dAngle < -MAX_COUNT){
			dAngle += ENCODER_TIM_PERIOD;
	}
	lastCount1 = curCount;
	return (s16)dAngle;
}

s16 ENC2_Get_Electrical_Angle(void)
{
//	static u16  lastCount2 = 0;
	u16 curCount = TIM3->CNT;
	s32 dAngle = curCount - lastCount2;

	if(dAngle >= MAX_COUNT){
			dAngle -= ENCODER_TIM_PERIOD;
	}
	else if(dAngle < -MAX_COUNT){
			dAngle += ENCODER_TIM_PERIOD;
	}
	lastCount2 = curCount;
	return (s16)dAngle; 
}


