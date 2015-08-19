#include "stm32f10x.h"
#include "Motor.h"
#include "QEI.h"


void GPIO_Configuration_QEI(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0; // TIM2 CH1 CH2	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}	

void QEI_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//------------------------------------------------------------------------------------------------------//	
	TIM_TimeBaseStructure.TIM_Period = EncoderPeriod;  // 348.4*4 = 1393.6  电机旋转一周编码器输出码值*4 
	TIM_TimeBaseStructure.TIM_Prescaler=0; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //计数方式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化

//------------------------------------------------------------------------------------------------------//
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
																									// 编码器3模式 TI1 TI2均捕获上升沿
//------------------------------------------------------------------------------------------------------//	
	
	TIM_Cmd(TIM2,ENABLE);
}

void QEI_Init(void)
{
	GPIO_Configuration_QEI();
	QEI_Configuration();
}





