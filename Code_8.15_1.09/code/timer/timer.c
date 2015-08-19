
/***********************************

ͨ��Timer6�Ե����ֵ���ڲ���

��������Ӧ����Ҫ����  ��ͷ�ļ���


*******************************/

#include "stm32f10x.h"
#include "Motor.h"
#include "timer.h"
#include "mpu6050.h"
#include "IMU.h"
#include "oled.h"

uint16_t timerCount = 0, timerFlag = 0;
float angle_speed_last = 0; 
uint8_t angle_speed_dir = 0;
float AngleMax_X = 0, AngleMin_X = 0;
float AngleMax_Y = 0, AngleMin_Y = 0;
uint8_t Angle_flag = 0;

void Timer3_Init(void)
{
	Timer3_Configuartion();
	NVIC_Configuartion_TIM3();
}

void Timer3_Configuartion(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	 
   
    TIM_DeInit(TIM3);
	
    TIM_TimeBaseStructure.TIM_Period= (uint16_t)(SamplePeriod)-1;  								//����ֵ Arr
    TIM_TimeBaseStructure.TIM_Prescaler=72-1; 									//ʱ��Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//ʱ��ָ�ֵ
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //������ʽ

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); //��ʼ��

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);   //�����־

		TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);
	
    TIM_Cmd(TIM3, ENABLE); //ʹ��TIM6
}

void NVIC_Configuartion_TIM3(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;//����ṹ�����

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//�������ȷּ���
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  //ͨ��
	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	
  //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;		
  //�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;			
  //ʹ���ж���������
  NVIC_Init(&NVIC_InitStructure);			
  //���������ó�ʼ���ж�����
	
}


void TIM3_IRQHandler(void)
{
//	float temp_speedNow=0, temp_speedlast=0;
	
  if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);	//���жϡ���
		
//	 angle_speed_last = MPU6050_GYRO_LAST.X;
		
//		if(angle_speed_last<0) temp_speedlast = -1*angle_speed_last;
//		else	temp_speedlast = angle_speed_last;
		
	//��ȡMPU6050����������̬
	if(Read_Filter_MPU6050()) 
		IMUupdate(&MPU6050_GYRO_LAST,&MPU6050_ACC_LAST,&Angle_Last);
	
	
//	if(MPU6050_GYRO_LAST.X < 0)	temp_speedNow = -1*MPU6050_GYRO_LAST.X;
//	else temp_speedNow = MPU6050_GYRO_LAST.X;
	
//	if(temp_speedNow > angle_speed_last)	angle_speed_dir = 1;
//	else	angle_speed_dir = 0;
	timerFlag = 1;
		
		
	timerCount++;
	if(Angle_Last.rol > AngleMax_X) AngleMax_X = Angle_Last.rol;
	if(Angle_Last.rol < AngleMin_X) AngleMin_X = Angle_Last.rol;
	
	if(Angle_Last.pit > AngleMax_Y) AngleMax_Y = Angle_Last.pit;
	if(Angle_Last.pit < AngleMin_Y) AngleMin_Y = Angle_Last.pit;
	
//		
		if(timerCount >= 800)
		{
			timerCount = 0;
			Angle_flag = 1;
		
		}
	}
}



