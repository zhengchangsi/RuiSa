#include "stm32f10x.h"
#include "board.h"
#include "pwm.h"
#include "Motor.h"
#include "delay.h"
#include "uart_config.h"
#include "timer.h"
#include "filter.h"
#include "stdio.h"
#include "mpu6050.h"
#include "IMU.h"
#include "oled.h"


/*******************************************************

目前暂时都是开环控制，没有闭环



******************************************************/
int main()
{
	float AngleRight = 0, AngleLift = 0;
	uint8_t flag = 0;
	
	
	delay_init(72);	
	Board_Init();
	MPU6050_Init();
	PWM_Init();
	OLED_Init();
	
//	
//=================================================//
// 初始化.... 风扇全都工作，免去启动长延时等待	
	Motor_Run(Motor_1, 0.2);
	Motor_Run(Motor_2, 0.2);
	Motor_Run(Motor_3, 0.2);
	Motor_Run(Motor_4, 0.2);
	delay_ms(1000);
	delay_ms(1000);
	
	Timer3_Init();
//=================================================//

	OLED_P8x16Str(0, 0, "Stone");
	OLED_P8x16Str(0, 2, "Y");
	OLED_P8x16Str(0, 4, "X");
	OLED_P8x16Str(0, 6, "pit:");
	OLED_P8x16Str(0, 6, "pit:        ");
	
	while(1)
	{
		AnglePendulum_CC(330);
//		StopPendulum_Y();
//		Motor_Run(Motor_1, 0);
//		Motor_Run(Motor_2, 0);
//		Motor_Run(Motor_3, 0.5);
//		Motor_Run(Motor_4, 0.1);
	}
	
	
	while(1)
	{
/***************************测试代码***********************************/
		
//		if(Angle_flag)
//		{
//			
//			Angle_flag = 0;
//			OLED_Shownumf(10, 2, 1, AngleMax_X);
//			OLED_Shownumf(10, 4, 1, AngleMin_X);
//			AngleMax_X = 0;
//			AngleMin_X = 0;
//			
//			
//		}
//		OLED_Shownumf(34, 6, 1, Angle_Last.rol);
//		max_p_ang_X(&AngleRight, &AngleLift);

//		Circle_CC(12.8);
//		 SimplePendulum_X_CC(18.8);
//		SimplePendulum_Y_CC(18.8);
//		SimplePendulum_X();			// X轴摆动
//		SimplePendulum_Y();		// Y轴摆动
//		AnglePendulum();			// 斜角摆动 ...效果不好  风机风力没有校正
//		Circle();							// 圆周.. 有可行性

/***************************测试代码***********************************/
	}
	
}
