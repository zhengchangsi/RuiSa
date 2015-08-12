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
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;						//PC6.7��ΪTIM3��ͨ��1.2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);			// TIM3��ȫ���ù��ܿ���	
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;						//PA0.1��ΪTIM2��ͨ��1.2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Enconder_init(void)
{
//	RCC->APB2ENR |= (1 << 2) | (1 << 0);	//����GPIOA��AFIOʱ��
	Enconder_IO_Init();
	
	TIM2->CCMR1 = 0x101;	//����T1=CH1 T2=CH2������ģʽ 
	TIM2->CCER = 0x00;	//���ü���Ϊ��
	TIM2->SMCR = 0x03;	//����Ϊ��ģʽ��������3ģʽ

	TIM2->ARR = 0xffff;	//����Ϊ��󣬱���Ƶ����װ��ֵ
	TIM2->PSC = 0x00;	//���÷�ƵΪ0
	
	TIM2->DIER |= 1<<0;
	TIM2->DIER |= 1<<6;
	TIM2->CR1	|= 0X01;

	TIM2->CR1 |= 1 << 7;	//����Ӱ�ӼĴ���
	TIM2->CR1 |= 1 << 0;	//������ʱ��
	
	
	TIM3->CCMR1 = 0x101;	//����T1=CH1 T2=CH2������ģʽ 
	TIM3->CCER = 0x00;	//���ü���Ϊ��
	TIM3->SMCR = 0x03;	//����Ϊ��ģʽ��������3ģʽ

	TIM3->ARR = 0xffff;	//����Ϊ��󣬱���Ƶ����װ��ֵ
	TIM3->PSC = 0x00;	//���÷�ƵΪ0
	
	TIM3->DIER |= 1<<0;
	TIM3->DIER |= 1<<6;
	TIM3->CR1	|= 0X01;
	
	
	TIM3->CR1 |= 1 << 7;	//����Ӱ�ӼĴ���
	TIM3->CR1 |= 1 << 0;	//������ʱ��
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


