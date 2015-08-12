#include "timer.h"
#include "enconder.h"
//extern s16 ENC1_Get_Electrical_Angle(void);
//extern s16 ENC2_Get_Electrical_Angle(void);

u8 Timer_Counter = 0;//�ٶ�ƽ��������
volatile s16 Back_Speed;//�ٶȷ���ֵ
u8  flag_speed = 0;//�ٶȴ����־λ
s32 Enconder_count1;//�������ۼ�ֵ
s32 Enconder_count2;	

//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM4_PWM_Init(u16 arr, u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8; //TIM3_CH1  TIM3_CH2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
    //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
 
  /*����PWMռ�ձ�*/
  TIM_SetCompare1(TIM4, 0);	
	TIM_SetCompare3(TIM4, 0);	
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

void Timer1_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(psc-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
//	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
//		TIM1, //TIM2
//		TIM_IT_Update  |  //TIM �ж�Դ
//		TIM_IT_Trigger,   //TIM �����ж�Դ 
//		ENABLE  //ʹ��
//	);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

 
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���						 
}
float test1, test2;
void TIM1_UP_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM1, TIM_FLAG_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			s16 bmq_lef = 0, bmq_right = 0;//���ұ�������ֵ
			TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			
			//�ٶ�ƽ��������
			Timer_Counter++;
			
			//��ȡ��������ֵ
			bmq_lef   = ENC1_Get_Electrical_Angle();
			bmq_right = ENC2_Get_Electrical_Angle();
			
			//�Ա�������ֵ�ۼ�
			Enconder_count1 += bmq_lef;
			Enconder_count2 += bmq_right;
			
			if(20 == Timer_Counter)
			{
					Timer_Counter = 0;
					Back_Speed = (Enconder_count1 + Enconder_count2)>>2;
				  test1 = Enconder_count1;
				  test2 = Enconder_count2;
				  Enconder_count1 = 0;//������ֵ����
				  Enconder_count2 = 0;
				
					flag_speed = 1; //��λ�ٶ�ת����־λ
			}
		}
}