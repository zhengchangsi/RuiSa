#include <stm32f10x.h>
#include <stdint.h>
#include <math.h>

#include "IQmath/IQmathLib.h"
#include "mpu6050.h"
#include "delay.h"
#include "timer.h"
#include "misc.h"
#include "enconder.h"
#include "led.h"
#include "control.h"
#include "uart_config.h"
#include "adc.h"
#include "IMU.h"

#define  Dead_Voltage     10     //������ѹ
#define  Speed_Conversion 0.257 //������ת��ת����ϵ

_PID  PID_angular_velocity;   //�ڻ����ٶ�PID
_PID  PID_angle_pose;         //�⻷��̬PID
_PID  PID_pose;               //λ�û�PID��������

u8 CAR_Status;								//ң����
s16 DV;                       //ǰ�����˵�����ֵ��ң����
s16 LIFT,RIGHT;               //ת��ֵ��ң����
float BAT;										//��ص���

u8 data_to_send[0xff];				//��̬����ʱ�õĻ���


//���ڳ�ʼ��PIDֵ
void PID_init(_PID* PID,float Kp,float Ki,float Kd)
{
	PID->SetVal=45;
	PID->ActualVal=0;
	PID->err=0;
	PID->err_last=0;
	PID->integral=0;
	PID->Kp=Kp;
	PID->Ki=Ki;
	PID->Kd=Kd;
}



//��̬PID����
//E �ǵ�ǰ��̬�Ƕ�ֵ
//set ϣ��С���ȶ���ĽǶ�
float PID_realize_angle_pose(_PID* PID, float E,float set)
{
	PID->SetVal = set;
	PID->ActualVal=E;
	
	PID->err = PID->ActualVal-PID->SetVal;
	PID->integral += PID->err;
	PID->integral = (PID->integral> 10000)?  10000:PID->integral;
	PID->integral = (PID->integral<-10000)? -10000:PID->integral;
	PID->voltage = PID->Kp * PID->err 
								 + PID->Ki * PID->integral 
								 + PID->Kd *(PID->err - PID->err_last);
	PID->err_last=PID->err;
	
	
	return PID->voltage ;
}

//���ٶ�PID����         λ��ʽPID
//angular_velocity   ��ǰ���ٶ�
//set                ϣ��С�����ֵĽ��ٶ�
float PID_realize_angular_velocity(_PID* PID, float angular_velocity,float set)
{
	PID->SetVal = set;
	
	PID->ActualVal=angular_velocity;
	
	//���
	PID->err = PID->ActualVal-PID->SetVal;
	
	//������
	PID->integral += PID->err;
	
	//�����޷�
	PID->integral = (PID->integral>5000) ?  5000:PID->integral;
	PID->integral = (PID->integral<-5000)? -5000:PID->integral;
	
	PID->voltage   = PID->Kp * PID->err 
								 + PID->Ki * PID->integral 
								 + PID->Kd *(PID->err - PID->err_last);

	//��һ�ε����
	PID->err_last=PID->err;
	
	return PID->voltage ;
}
//λ�û�  ��Ҫ�޸ĺ����
float PID_realize_pose(_PID* PID, float angular_velocity,float set)
{
	PID->SetVal = set;
	PID->ActualVal=angular_velocity;
	
	PID->err = PID->ActualVal-PID->SetVal;
	PID->integral += PID->err;
	
	PID->integral = (PID->integral>100000) ?  100000:PID->integral;
	PID->integral = (PID->integral<-100000)? -100000:PID->integral;
	
	
	/*************************���ֱ���ָʾ***************************/ //���������������
	if(PID->integral == 100000 || PID->integral == -100000)
	{
		LED_ON();
	}
	else
	{
		LED_OFF();
	}
	/*************************���ֱ���ָʾ***************************/
	
	PID->voltage = PID->Kp * PID->err 
								 + PID->Ki * PID->integral 
								 + PID->Kd *(PID->err - PID->err_last);
	PID->err_last=PID->err;
	
	return PID->voltage ;
}

void Usart_Send_Str(u8* Temp,u8 num)
{
	u8 i=0;
	for (i=0;i<num;i++)
	{
		USART_SendData(USART1,Temp[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}
}




//������λ��ʹ��
void Uart3_Send_RCdata()
{
	uint8_t sum = 0;
	uint8_t cnt = 0;
	uint8_t i;
	
	data_to_send[cnt++]=0xAA;
	data_to_send[cnt++]=0xAA;
	data_to_send[cnt++]=0x02;
	data_to_send[cnt++]=0;

//	data_to_send[cnt++]=BYTE1(MPU6050_ACC_LAST.X);
//	data_to_send[cnt++]=BYTE0(MPU6050_ACC_LAST.X);
	data_to_send[cnt++]=BYTE1(MPU6050_ACC_LAST.Y);
	data_to_send[cnt++]=BYTE0(MPU6050_ACC_LAST.Y);
//	data_to_send[cnt++]=BYTE1(MPU6050_ACC_LAST.Z);
//	data_to_send[cnt++]=BYTE0(MPU6050_ACC_LAST.Z);
//	
	data_to_send[cnt++]=BYTE1(MPU6050_GYRO_LAST.X);
	data_to_send[cnt++]=BYTE0(MPU6050_GYRO_LAST.X);
//	data_to_send[cnt++]=BYTE1(MPU6050_GYRO_LAST.Y);
//	data_to_send[cnt++]=BYTE0(MPU6050_GYRO_LAST.Y);
//	data_to_send[cnt++]=BYTE1(MPU6050_GYRO_LAST.Z);
//	data_to_send[cnt++]=BYTE0(MPU6050_GYRO_LAST.Z);

	data_to_send[3]=cnt-4;
	
	for (i=0;i<cnt;i++)
	{
		sum += data_to_send[i];
	}
		data_to_send[cnt++] = sum;
		Usart_Send_Str(data_to_send,cnt);
}

void Data_Send_Status(void)
{
	u8 _cnt=0;
	u8 i;
	s16 _temp;
	u8 sum = 0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(Angle_Last.rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);

	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
	sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	Usart_Send_Str(data_to_send,_cnt);
}

s16 Speed_One=3000, Speed_Thr = 600;
float angle_count = 0;
int main(void)
{
	uint8_t huanxiang = 1, flag = 0;
	u16 i=0;          //���ڵ�ص����ı���
	float set_pit = 0, set_rol = 0;//����rol��pit��ֵ
	float dif_pit, dif_rol;//rol��pit�����ֵ
	float last_pit;
	s16 Change = 0;
  s16 /*Speed_One=0, Speed_Thr = 0, */Motor_Speed=0;     //��ż��������PWMֵ
	
	
 	NVIC_SetPriority(SysTick_IRQn, 0x00);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Timer1_Init(19, 7199); //10Khz�ļ���Ƶ��,������50Ϊ5ms(��ʱ��3��ʼ��)
	LED_Init();
	LED_OFF();
	MPU6050_Init();
	Control_gpio_init();
	Enconder_init();
	TIM4_PWM_Init(TIM4_CONT-1,1-1); //PWM3��ʼ���������������PWMƵ��10Khz	
	UART_Init(9600);
	Adc_Init();
	
	
	PID_init(&PID_angular_velocity,12,0,30); //���ٶ��ڻ�һ�㲻�ӻ��֣� MPU6050�Ľ��ٶȻ�Ư�ƣ� ���ֻᵼ��С�����ȶ�
  PID_init(&PID_angle_pose,18 ,0.040 ,20); //�Ƕ��⻷,��̬����
//	PID_init(&PID_angle_pose,4 ,0.040 ,20); //�Ƕ��⻷,��̬����
	PID_init(&PID_pose,0.2,0.012,0.08);      //λ���⻷ (200�ٶ� 0.6 0.05  0)  (500 0.28 0.02  0.12) (800 0.2 0.012 0.08)
	
	Beep_warning(); //��ʼ���������
	
	
/*________________����Ϊ��ڳ���__________________*/
  Speed_One = 600;
	Speed_Thr = 600;
  Motor_Speed_Ctrl(Speed_One,MOTOR_NUM_A);	
	Motor_Speed_Ctrl(Speed_Thr,MOTOR_NUM_B);
	delay_ms(10000);
	Speed_One = 600;
	Speed_Thr = 3000;
  Motor_Speed_Ctrl(Speed_One,MOTOR_NUM_A);	
	Motor_Speed_Ctrl(Speed_Thr,MOTOR_NUM_B);
	delay_ms(5500);
	Speed_One = 3000;
	Speed_Thr = 600;
	Motor_Speed_Ctrl(Speed_One,MOTOR_NUM_A);	
	Motor_Speed_Ctrl(Speed_Thr,MOTOR_NUM_B);
	delay_ms(5900);
	Speed_One = 600;
	Speed_Thr = 3000;
	Motor_Speed_Ctrl(Speed_One,MOTOR_NUM_A);	
	Motor_Speed_Ctrl(Speed_Thr,MOTOR_NUM_B);
	delay_ms(7000);
	Speed_One = 3000;
	Speed_Thr = 600;
	Motor_Speed_Ctrl(Speed_One,MOTOR_NUM_A);	
	Motor_Speed_Ctrl(Speed_Thr,MOTOR_NUM_B);
	delay_ms(7000);
	Speed_One = 600;
	Speed_Thr = 3000;
	Motor_Speed_Ctrl(Speed_One,MOTOR_NUM_A);	
	Motor_Speed_Ctrl(Speed_Thr,MOTOR_NUM_B);
	delay_ms(7000);
	Speed_One = 600;
	Speed_Thr = 3000;
	Motor_Speed_Ctrl(Speed_One,MOTOR_NUM_A);	
	Motor_Speed_Ctrl(Speed_Thr,MOTOR_NUM_B);
	delay_ms(500);
/*________________����Ϊ��ڳ���__________________*/
	while(1)
	{
		//��ȡMPU6050����������̬
		if(Read_Filter_MPU6050()) IMUupdate(&MPU6050_GYRO_LAST,&MPU6050_ACC_LAST,&Angle_Last);      //��ȡMPU6050����������̬
/*______________________����Ϊ������Ƴ���__________________________*/
//		if((MPU6050_ACC_LAST.Y <  150 && MPU6050_ACC_LAST.Y > 0))  {Speed_Thr = 3000;  Speed_One = 600; }
//		if((MPU6050_ACC_LAST.Y > -150 && MPU6050_ACC_LAST.Y < 0))  {Speed_One = 3000;  Speed_Thr = 600;}
		
		/*_________________����Σ��������ֹͣ����________________*/
		if((MPU6050_GYRO_LAST.Y<150 && MPU6050_GYRO_LAST.Y>-150) || MPU6050_GYRO_LAST.Y > 1200 || MPU6050_GYRO_LAST.Y < -1200)  
		{
					Speed_One = 600;
					Speed_Thr = 600;
		}
		else if(MPU6050_GYRO_LAST.Y > 150 && MPU6050_GYRO_LAST.Y < 1200)//���г�,��ȫ����
		{
					Speed_One = 600;
					Speed_Thr = 3000;
		}
		else if(MPU6050_GYRO_LAST.Y < -150 && MPU6050_GYRO_LAST.Y > -1200)//���г�,��ȫ����
		{
					Speed_One = 3000;
					Speed_Thr = 600;
		}
/*______________________����Ϊ������Ƴ���__________________________*/
		
//			Motor_Speed_Ctrl(Speed_One,MOTOR_NUM_A);	
//			Motor_Speed_Ctrl(Speed_Thr,MOTOR_NUM_B);
				Motor_Speed_Ctrl(Speed_One,MOTOR_NUM_A);	
				Motor_Speed_Ctrl(Speed_Thr,MOTOR_NUM_B);
	}
}

