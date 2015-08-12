#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************

#define TIM4_CONT 3000       //50KHZ的初值
#define P_CONT TIM4_CONT/100         //

//速度平滑处理常数
extern u8 Timer_Counter;

//速度反馈值
extern volatile s16 Back_Speed;

//速度控制转化标志位
extern u8  flag_speed;

void TIM4_PWM_Init(u16 arr,u16 psc);

//定时器1初始化
void Timer1_Init(u16 arr,u16 psc);
//TIM1 中断
void TIM1_UP_IRQHandler(void);   //TIM1中断

#endif
