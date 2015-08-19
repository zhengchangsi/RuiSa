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
	
	TIM_TimeBaseStructure.TIM_Period=5999;   //����ֵ Arr
	TIM_TimeBaseStructure.TIM_Prescaler=719; //TIM2ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //ʱ��ָ�ֵ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //������ʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //��ʼ��
		
//========================================================================================================//	

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; 				//ͨ��2
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//�������ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	//
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	
	TIM_ICInitStructure.TIM_ICFilter = 0x00; 					//�˲����ã������������������϶������ȶ�0x0��0xF
	
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
//	TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);
	
/*========================================================================================================*/
	
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);	//ѡ��tim2���봥��Դ��tim���˲���ʱ������2

/*========================================================================================================*/
	
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset); 			//ѡ��ӻ�ģʽ�������ظ�λ������
	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable); //ʹ��
	
/*========================================================================================================*/	
	//TIM_SetCompare2(TIM2, 0); //���ò�����������ֵ
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);

	TIM_Cmd(TIM2,ENABLE);
}

void NVIC_Configuration_Catch(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;//����ṹ�����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//�������ȷּ��� 
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	//ѡ���ж�ͨ����ע�⣺�̼�����ΪXXX_IRQChannel�����ó���Ԥ����Ϊ XXX_IRQn������Ҫ�ر�ע�� 
	//lib V3.5 Ϊxxx_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�	  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;	//�����ȼ�	
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	//ʹ���ж���������		
	
	NVIC_Init(&NVIC_InitStructure);//���������ó�ʼ���ж�����															   
}

void TIM2_IRQHandler()
{
	TIM2_Cap2 = TIM_GetCapture2(TIM2);
	MotorA.	SpeedMeasured = TIM2_Cap2;
	
//---------------------------------------------------------------//

//---------------------------------------------------------------//	
	TIM2Cap_flag = 1;   		// �����־λ
//---------------------------------------------------------------//	 

//---------------------------------------------------------------//	
	//MotorA.SpeedMeasured = (float)PeriodMin/MotorA.EnA_Period;
//---------------------------------------------------------------//	
	TIM_ClearITPendingBit(TIM2,TIM_FLAG_CC2);	//���ж� 
}





