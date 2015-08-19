#include "stm32f10x.h"
#include "Catch.h"
#include "Motor.h"

uint16_t TIM2_Cap1 = 0, TIM2_Cap2 = 0;
uint8_t TIM2Cap_flag = 0;


void GPIO_Configuration_Catch(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0; // TIM2 CH2	 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void Catch_Configuration(void)
{
//========================================================================================================//	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;	
	
//========================================================================================================//	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
//========================================================================================================//	
	
	TIM_TimeBaseStructure.TIM_Period=5999;   //计数值 Arr
	TIM_TimeBaseStructure.TIM_Prescaler=719; //TIM2时钟预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //时间分割值
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //计数方式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化
		
//========================================================================================================//	

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; 				//通道2
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//上升沿沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	
	TIM_ICInitStructure.TIM_ICFilter = 0x00; 					//滤波设置，经历几个周期跳变认定波形稳定0x0～0xF
	
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
//	TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);
	
/*========================================================================================================*/
	
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);	//选择tim2输入触发源：tim经滤波定时器输入2

/*========================================================================================================*/
	
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset); 			//选择从机模式，上升沿复位计数器
	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable); //使能
	
/*========================================================================================================*/	
	//TIM_SetCompare2(TIM2, 0); //设置捕获器计数初值
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);

	TIM_Cmd(TIM2,ENABLE);
}

void NVIC_Configuration_Catch(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;//定义结构体变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//设置优先分级组 
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	//选择中断通道。注意：固件库中为XXX_IRQChannel，但该程序预定义为 XXX_IRQn，所以要特别注意 
	//lib V3.5 为xxx_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//先占优先级	  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//从优先级	
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//使能中断向量配置		
	
	NVIC_Init(&NVIC_InitStructure);//按以上设置初始化中断向量															   
}

void TIM2_IRQHandler()
{
	TIM2_Cap2 = TIM_GetCapture2(TIM2);
	MotorA.	SpeedMeasured = TIM2_Cap2;
	
//---------------------------------------------------------------//

//---------------------------------------------------------------//	
	TIM2Cap_flag = 1;   		// 捕获标志位
//---------------------------------------------------------------//	 

//---------------------------------------------------------------//	
	//MotorA.SpeedMeasured = (float)PeriodMin/MotorA.EnA_Period;
//---------------------------------------------------------------//	
	TIM_ClearITPendingBit(TIM2,TIM_FLAG_CC2);	//清中断 
}





