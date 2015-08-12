#include "timer.h"
#include "enconder.h"
//extern s16 ENC1_Get_Electrical_Angle(void);
//extern s16 ENC2_Get_Electrical_Angle(void);

u8 Timer_Counter = 0;//速度平滑处理常数
volatile s16 Back_Speed;//速度反馈值
u8  flag_speed = 0;//速度处理标志位
s32 Enconder_count1;//编码器累加值
s32 Enconder_count2;	

//这里时钟选择为APB1的2倍，而APB1为36M
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM4_PWM_Init(u16 arr, u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8; //TIM3_CH1  TIM3_CH2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
    //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	
 
  /*设置PWM占空比*/
  TIM_SetCompare1(TIM4, 0);	
	TIM_SetCompare3(TIM4, 0);	
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

void Timer1_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(psc-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
//	TIM_ITConfig(  //使能或者失能指定的TIM中断
//		TIM1, //TIM2
//		TIM_IT_Update  |  //TIM 中断源
//		TIM_IT_Trigger,   //TIM 触发中断源 
//		ENABLE  //使能
//	);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

 
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器						 
}
float test1, test2;
void TIM1_UP_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM1, TIM_FLAG_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			s16 bmq_lef = 0, bmq_right = 0;//左右编码器的值
			TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);  //清除TIMx的中断待处理位:TIM 中断源 
			
			//速度平滑处理常数
			Timer_Counter++;
			
			//读取编码器的值
			bmq_lef   = ENC1_Get_Electrical_Angle();
			bmq_right = ENC2_Get_Electrical_Angle();
			
			//对编码器的值累加
			Enconder_count1 += bmq_lef;
			Enconder_count2 += bmq_right;
			
			if(20 == Timer_Counter)
			{
					Timer_Counter = 0;
					Back_Speed = (Enconder_count1 + Enconder_count2)>>2;
				  test1 = Enconder_count1;
				  test2 = Enconder_count2;
				  Enconder_count1 = 0;//编码器值清零
				  Enconder_count2 = 0;
				
					flag_speed = 1; //置位速度转换标志位
			}
		}
}