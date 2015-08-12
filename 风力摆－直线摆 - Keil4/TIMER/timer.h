#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************

#define TIM4_CONT 3000       //50KHZ�ĳ�ֵ
#define P_CONT TIM4_CONT/100         //

//�ٶ�ƽ��������
extern u8 Timer_Counter;

//�ٶȷ���ֵ
extern volatile s16 Back_Speed;

//�ٶȿ���ת����־λ
extern u8  flag_speed;

void TIM4_PWM_Init(u16 arr,u16 psc);

//��ʱ��1��ʼ��
void Timer1_Init(u16 arr,u16 psc);
//TIM1 �ж�
void TIM1_UP_IRQHandler(void);   //TIM1�ж�

#endif
