#include "stm32f10x.h"
#include "Motor.h"
#include "pwm.h"
#include "timer.h"
#include "filter.h"
#include "mpu6050.h"
#include "IMU.h"
#include "delay.h"
#include "myPID.h"
#include "oled.h"
#include "math.h"

Motor MotorA;
Motor MotorB;
Motor MotorC;
Motor MotorD;

#define PerchLength 87.8

void Motor_Run(uint8_t Motorx, float speed)
{
	PWM_Pulse_Change(Motorx, (uint16_t)(D_TIM1_period*speed));
}


void max_p_ang_Y(float *mz, float *mf)  //y轴方向
{
	volatile static int g_x_last = 1, cnt = 0;
	volatile static float max_z = 0, max_f = 0;
				
	if(g_x_last * MPU6050_GYRO_LAST.X <= 0)
	{
		if(MPU6050_GYRO_LAST.X < 40 && MPU6050_GYRO_LAST.X > -40)
		{	
			if(MPU6050_GYRO_LAST.X >= 0)
			{
				*mf = max_f;
				max_f = 0;
				cnt = 0;
//				printf("max_x_f\r\n");
			}
			else if(MPU6050_GYRO_LAST.X < 0)
			{
				*mz = max_z;	
				max_z = 0;
				cnt = 0;
//				printf("max_x_z\r\n");
			}
		}
	}
	else
	{
		if(Angle_Last.rol > max_z)
		{
			max_z = Angle_Last.rol;
		}
		if(Angle_Last.rol < max_f)
		{
			max_f = Angle_Last.rol;	
		}	
	}
	g_x_last = MPU6050_GYRO_LAST.X;	
	if(cnt++>=1500) 
	{
		cnt = 0;
		*mz = 0;
		*mf = 0;
	}	
}


void max_p_ang_X(float *mz, float *mf)  //x轴方向
{
	volatile static int g_x_last = 1, cnt = 0;
	volatile static float max_z = 0, max_f = 0;
				
	if(g_x_last * MPU6050_GYRO_LAST.Y <= 0)
	{
		if(MPU6050_GYRO_LAST.Y < 40 && MPU6050_GYRO_LAST.Y > -40)
		{	
			if(MPU6050_GYRO_LAST.Y >= 0)
			{
				*mf = max_f;			
				max_f = 0;
				cnt = 0;
			}
			else if(MPU6050_GYRO_LAST.Y < 0)
			{
				*mz = max_z;
				max_z = 0;
				cnt = 0;
			}
		}
	}
	else
	{
		if(Angle_Last.pit > max_z)
		{
			max_z = Angle_Last.pit;
		}
		if(Angle_Last.pit < max_f)
		{
			max_f = Angle_Last.pit;	
		}	
	}
	g_x_last = MPU6050_GYRO_LAST.Y;
	if(cnt++>=1500) 
	{
		cnt = 0;
		*mz = 0;
		*mf = 0;
	}		
}

void SimplePendulum_X(void)
{
//==========================================//
// 摆杆初始起摆
	Motor_Run(Motor_2, 1);
	Motor_Run(Motor_4, 0);
	delay_ms(1000);
	
	Motor_Run(Motor_2, 0);
	Motor_Run(Motor_4, 1);
	delay_ms(500);
//=========================================//

	Motor_Run(Motor_1, 0.1);
	Motor_Run(Motor_2, 0.1);
	Motor_Run(Motor_3, 0.1);
	Motor_Run(Motor_4, 0.1);
	
	while(1)
	{
		if(timerFlag == 1)	// 定时器采样标志位
		{	
			timerFlag = 0;
			
			if(Angle_Last.rol > 4 && Angle_Last.rol < 10)						
			{
				if(MPU6050_GYRO_LAST.X > 0)
				{
					Motor_Run(Motor_4, 1);
					Motor_Run(Motor_2, 0.1);
					delay_ms(500);
					Motor_Run(Motor_4, 0.1);
				}
				else
				{
					Motor_Run(Motor_4, 0.1);
					Motor_Run(Motor_2, 0.1);
				}				
			}
			else if(Angle_Last.rol >-10 && Angle_Last.rol < -4)
			{
				if(MPU6050_GYRO_LAST.X < 0)
				{
					Motor_Run(Motor_4, 0.1);
					Motor_Run(Motor_2, 1);
					delay_ms(500);
					Motor_Run(Motor_2, 0.1);
				}
				else
				{
					Motor_Run(Motor_4, 0.1);
					Motor_Run(Motor_2, 0.1);
				}
			}
		}
	}
}


void SimplePendulum_Y(void)
{
//==========================================//
// 摆杆初始起摆
	Motor_Run(Motor_1, 1);
	Motor_Run(Motor_3, 0);
	delay_ms(1000);
	
	Motor_Run(Motor_1, 0);
	Motor_Run(Motor_3, 1);
	delay_ms(500);
//=========================================//

	Motor_Run(Motor_1, 0.1);
	Motor_Run(Motor_2, 0.1);
	Motor_Run(Motor_3, 0.1);
	Motor_Run(Motor_4, 0.1);
	
	while(1)
	{
		if(timerFlag == 1)	// 定时器采样标志位
		{	
			timerFlag = 0;
			
			if(Angle_Last.pit > 4 && Angle_Last.pit < 10)						
			{
				if(MPU6050_GYRO_LAST.Y < 0)
				{
					Motor_Run(Motor_3, 1);
					Motor_Run(Motor_1, 0.1);
					delay_ms(500);
					Motor_Run(Motor_3, 0.1);
				}
				else
				{
					Motor_Run(Motor_3, 0.1);
					Motor_Run(Motor_1, 0.1);
				}				
			}
			else if(Angle_Last.pit < -4 && Angle_Last.pit > -10)
			{
				if(MPU6050_GYRO_LAST.Y > 0)
				{
					Motor_Run(Motor_3, 0.1);
					Motor_Run(Motor_1, 1);
					delay_ms(500);
					Motor_Run(Motor_1, 0.1);
				}
				else
				{
					Motor_Run(Motor_3, 0.1);
					Motor_Run(Motor_1, 0.1);
				}
			}
		}
	}
}

void SimplePendulum_Y_CC(float GlobalAngle)
{
	float AngleRight = 0, AngleLift = 0, AngleAver = 0;
	float SpeedMotor3 = 1, SpeedMotor1 = 1, SpeedAver = 0;
	uint8_t flag = 0;
	static float Acc_last = 0, AccDir_last = 0;
	float Acc_now =  0, AccDir_now = 0;
	
//==========================================//
// 摆杆初始起摆
	Motor_Run(Motor_1, 1);
	Motor_Run(Motor_3, 0);
	delay_ms(500);
	
	Motor_Run(Motor_1, 0);
	Motor_Run(Motor_3, 1);
	delay_ms(300);
//=========================================//

	Motor_Run(Motor_1, 0.1);
	Motor_Run(Motor_2, 0.1);
	Motor_Run(Motor_3, 0.1);
	Motor_Run(Motor_4, 0.1);
	
	PID_Global = GlobalAngle;
	
	while(1)
	{
		if(timerFlag == 1)	// 定时器采样标志位
		{	
			timerFlag = 0;
			
//-----------------------------------------------------------------------------//
			
			if(Angle_Last.pit > 2 && Angle_Last.pit < 8 )					
			{
				if(MPU6050_GYRO_LAST.Y < 0)
				{
					if(SpeedAver >= 0)
					{
						Motor_Run(Motor_3, SpeedAver);
						Motor_Run(Motor_1, 0.1);
						delay_ms(300);
						Motor_Run(Motor_3, 0.1);
					}
					else
					{
						Motor_Run(Motor_3, 0.1);
						Motor_Run(Motor_1, -1*SpeedAver);
						delay_ms(300);
						Motor_Run(Motor_1, 0.1);
					}
				}			
			}
			else if(Angle_Last.pit < -2 && Angle_Last.pit > -8 )
			{
				if(MPU6050_GYRO_LAST.Y > 0)
				{
					if(SpeedAver>=0)
					{
						Motor_Run(Motor_3, 0.1);
						Motor_Run(Motor_1, SpeedAver);
						delay_ms(300);
						Motor_Run(Motor_1, 0.1);
					}
					else 
					{
						Motor_Run(Motor_1, 0.1);
						Motor_Run(Motor_3, -1*SpeedAver);
						delay_ms(300);
						Motor_Run(Motor_3, 0.1);
					}
				}
			}

			if(Angle_flag)
			{
				Angle_flag = 0;
	
				AngleRight = AngleMax_Y;
				AngleLift = AngleMin_Y;
//-------------------------------------------------------------------//					
				OLED_Shownumf(10, 2, 1, AngleMax_Y);
				OLED_Shownumf(10, 4, 1, AngleMin_Y);
//-------------------------------------------------------------------//				
				AngleMax_Y = 0;
				AngleMin_Y = 0;
				
				AngleAver = (AngleRight + AngleLift)/2.0;
				
				SpeedAver = PID_Regulator(AngleRight);
				if(SpeedAver > 1) SpeedAver = 1;
				else if(SpeedAver < -1) SpeedAver= -1;		
			}
		}
	}
}

void SimplePendulum_X_CC(float GlobalAngle)
{
	float AngleRight = 0, AngleLift = 0, AngleAver = 0;
	float SpeedMotor3 = 1, SpeedMotor1 = 1, SpeedAver = 0;
	uint8_t flag = 0;
	static float Acc_last = 0, AccDir_last = 0;
	float Acc_now =  0, AccDir_now = 0;
	
//==========================================//
// 摆杆初始起摆
	Motor_Run(Motor_2, 1);
	Motor_Run(Motor_4, 0);
	delay_ms(1000);
	
	Motor_Run(Motor_2, 0);
	Motor_Run(Motor_4, 1);
	delay_ms(500);
//=========================================//

	Motor_Run(Motor_1, 0.1);
	Motor_Run(Motor_2, 0.1);
	Motor_Run(Motor_3, 0.1);
	Motor_Run(Motor_4, 0.1);
	
	PID_Global = GlobalAngle;
	
	while(1)
	{
		if(timerFlag == 1)	// 定时器采样标志位
		{	
			timerFlag = 0;
			
//-----------------------------------------------------------------------------//
			
			if(Angle_Last.rol > 2 && Angle_Last.rol < 8 )					
			{
				if(MPU6050_GYRO_LAST.X > 0)
				{
					if(SpeedAver >= 0)
					{
						Motor_Run(Motor_4, SpeedAver);
						Motor_Run(Motor_2, 0.1);
						delay_ms(300);
						Motor_Run(Motor_4, 0.1);
					}
					else
					{
						Motor_Run(Motor_4, 0.1);
						Motor_Run(Motor_2, -1*SpeedAver);
						delay_ms(300);
						Motor_Run(Motor_2, 0.1);
					}
				}			
			}
			else if(Angle_Last.rol < -2 && Angle_Last.rol > -8 )
			{
				if(MPU6050_GYRO_LAST.X < 0)
				{
					if(SpeedAver>=0)
					{
						Motor_Run(Motor_4, 0.1);
						Motor_Run(Motor_2, SpeedAver);
						delay_ms(300);
						Motor_Run(Motor_2, 0.1);
					}
					else 
					{
						Motor_Run(Motor_2, 0.1);
						Motor_Run(Motor_4, -1*SpeedAver);
						delay_ms(300);
						Motor_Run(Motor_4, 0.1);
					}
				}
			}

			if(Angle_flag)
			{
				Angle_flag = 0;
				
				AngleRight = AngleMax_X;
				AngleLift = AngleMin_X;
//-------------------------------------------------------------------//				
				OLED_Shownumf(10, 2, 1, AngleMax_X);
				OLED_Shownumf(10, 4, 1, AngleMin_X);
//-------------------------------------------------------------------//					
				AngleMax_X = 0;
				AngleMin_X = 0;
				
				AngleAver = (AngleRight + AngleLift)/2.0;
				
				SpeedAver = PID_Regulator(AngleRight);
				if(SpeedAver > 1) SpeedAver = 1;
				else if(SpeedAver < -1) SpeedAver= -1;
			}	
		}
	}
}

// 斜角
void AnglePendulum(void)
{
	Motor_Run(Motor_1, 1);
	Motor_Run(Motor_2, 1);
	Motor_Run(Motor_3, 0);
	Motor_Run(Motor_4, 0);

	delay_ms(100);

	Motor_Run(Motor_1, 0.1);
	Motor_Run(Motor_2, 0.1);
	Motor_Run(Motor_3, 0.1);
	Motor_Run(Motor_4, 0.1);
	
	while(1){
		if(timerFlag == 1)
		{
			timerFlag = 0;
			
			if(Angle_Last.pit > 0.5 && Angle_Last.pit < 3)			
			{
				if(MPU6050_GYRO_LAST.Y < 0)
				{
					Motor_Run(Motor_3, 1);
					Motor_Run(Motor_4, 1);
					Motor_Run(Motor_1, 0.1);
					Motor_Run(Motor_2, 0.45);
					delay_ms(50);
					Motor_Run(Motor_3, 0.1);
					Motor_Run(Motor_4, 0.1);
				}
				else
				{
					Motor_Run(Motor_3, 0.1);
					Motor_Run(Motor_1, 0.1);
					Motor_Run(Motor_4, 0.1);
					Motor_Run(Motor_2, 0.1);
				}				
			}
//			else if(Angle_Last.pit < -2 && Angle_Last.pit > -6)
//			{
//				if(MPU6050_GYRO_LAST.Y > 0)
//				{
////					Motor_Run(Motor_3, 0.1);
////					Motor_Run(Motor_4, 0.1);
////					Motor_Run(Motor_1, 1);
////					Motor_Run(Motor_2, 1);
////					delay_ms(500);
////					Motor_Run(Motor_1, 0.1);
////					Motor_Run(Motor_2, 0.1);
//				}
//				else
//				{
//					Motor_Run(Motor_3, 0.1);
//					Motor_Run(Motor_1, 0.1);
//					Motor_Run(Motor_4, 0.1);
//					Motor_Run(Motor_2, 0.1);
//				}
//			}
		}
	}
}

void Get_XY(float Angle_X, float Angle_Y, float *X, float *Y)
{
	*X = (float)PerchLength*tan(Angle_X);
	*Y = (float)PerchLength*tan(Angle_Y);
}

void AnglePendulum_CC(float Angle)  // Angle定义为 以x轴正方向为起点按照逆时针方向转动的角度
{
	float Goal_y = 0, Goal_x = 0, k = 0;
	float SpeedMotor1 = 0, SpeedMotor2 = 0, SpeedMotor3 = 0, SpeedMotor4 = 0;
	float SpeedAver_Y = 0;
	uint8_t State = 0;
	float AngleTemp = 0;
	float AngleRight_X = 0, AngleLift_X = 0, AngleAver_X = 0, AngleSource_X = 0;
	float AngleRight_Y = 0, AngleLift_Y = 0, AngleAver_Y = 0, AngleSource_Y = 0;
	float coor_x = 0, coor_y = 0;
	uint8_t i = 0;
	
	AngleTemp = Angle;
	
	if(Angle == 0 || Angle ==  360)
	{
		SimplePendulum_X();
	}
	else if(Angle == 90 || Angle == 180)
	{
		SimplePendulum_Y();
	}
	else
	{
		k = tan(Angle);				// 目标直线为: Goal_y = k*Goal_x;
	}

	PID_Global = 0;  // 原点的期望值
	
	while(1)
	{
		if(timerFlag == 1)
		{
			timerFlag = 0;
			if((Angle > 0 && Angle < 90) || (Angle > 180 && Angle < 270))		// 第一三象限
			{
				if(Angle > 90) AngleTemp = Angle - 180;  // Angle:0~90
				if(AngleTemp <= 45)
				{
					SpeedMotor3 = AngleTemp/90.0;  // 0.1 ~ 0.9
					SpeedMotor4 = 0.5;
					SpeedMotor1 = 0;
					SpeedMotor2 = 0;
				}
				else
				{
					SpeedMotor3 = 0.5;
					SpeedMotor4 = (90-AngleTemp)/90.0;
					
					SpeedMotor1 = 0;
					SpeedMotor2 = 0;
				}
				State = 1;  // 状态1 第一三象限内摆动
			}
			else if((Angle > 90 && Angle < 180) || (Angle > 270 && Angle < 360)) // 第二四象限
			{
				if(Angle > 270) AngleTemp = Angle - 270;	// Angle: 0~90	
				else	AngleTemp = Angle - 90;
				
				if(AngleTemp <= 45)
				{
					SpeedMotor1 = 0.5;
					SpeedMotor4 = AngleTemp/90.0;  // 0.1 ~ 0.90.5;
					SpeedMotor2 = 0;
					SpeedMotor3 = 0;
				}
				else
				{
					SpeedMotor1 = (90-AngleTemp)/90.0;
					SpeedMotor4 = 0.5;
					SpeedMotor2 = 0;
					SpeedMotor3 = 0;
				}
				State = 2;  // 状态2 第二四象限内摆动
			}
//=================================================================================//
//  .............................	
//  PID控制  
		
			if(Angle_Last.rol <1 && Angle_Last.rol > -1)		// 过原点时  角度值
			{
				AngleSource_Y = Angle_Last.pit;
				Get_XY(AngleSource_X, AngleSource_Y, &coor_x, &coor_y);
			}
			
			max_p_ang_Y(&AngleRight_Y, &AngleLift_Y);		// 通过角速度选取最大角度
			max_p_ang_X(&AngleRight_X, &AngleLift_X);
//---------------------------------------------------------------------//			
			i++;
			if(i == 1)
			{
				SpeedAver_Y = PID_Regulator(coor_y);
			}
			else if(i == 2)
			{
				Get_XY(AngleRight_X, AngleRight_Y, &coor_x, &coor_y);
				SpeedAver_Y = PID_Regulator(coor_y);
			}
			else if(i == 3)
			{
				i = 0;
				Get_XY(AngleLift_X, AngleLift_Y, &coor_x, &coor_y);
				SpeedAver_Y = PID_Regulator(coor_y);
			}
			
			if(SpeedAver_Y > 1) SpeedAver_Y = 1;
			else if(SpeedAver_Y < -1) SpeedAver_Y = -1;
						
			
//=================================================================================//		
//Conctrol			
			if(State == 1)	// 一三象限
			{
				if(Angle_Last.rol)
			}
			else if(State == 2)	// 二四象限
			{
				
			}
//			if(Angle_Last.pit > 0.5 && Angle_Last.pit < 3)			
//			{
//				if(MPU6050_GYRO_LAST.Y < 0)
//				{
//					Motor_Run(Motor_3, SpeedMotor3);
//					Motor_Run(Motor_4, SpeedMotor4);
//					Motor_Run(Motor_1, SpeedMotor1);
//					Motor_Run(Motor_2, SpeedMotor2);
//					delay_ms(50);
//					Motor_Run(Motor_3, 0.1);
//					Motor_Run(Motor_4, 0.1);
//				}
//				else
//				{
//					Motor_Run(Motor_3, 0.1);
//					Motor_Run(Motor_1, 0.1);
//					Motor_Run(Motor_4, 0.1);
//					Motor_Run(Motor_2, 0.1);
//				}				
//			}
		}
	}
}

void Circle(void)
{
//==========================================//
// 摆杆初始起摆
	Motor_Run(Motor_1, 1);
	Motor_Run(Motor_2, 1);
	Motor_Run(Motor_3, 0);
	delay_ms(1000);
	Motor_Run(Motor_2, 0);
	Motor_Run(Motor_1, 0);
	Motor_Run(Motor_3, 1);
	delay_ms(500);
//=========================================//

	Motor_Run(Motor_1, 0.1);
	Motor_Run(Motor_2, 0.1);
	Motor_Run(Motor_3, 0.1);
	Motor_Run(Motor_4, 0.1);
	
	
	while(1)
	{
		if(timerFlag == 1)
		{
			timerFlag = 0;
			
//--------------------------------------------------------------------------------------//
			
			if(Angle_Last.pit > 2 && Angle_Last.pit < 8)						
			{
				if(MPU6050_GYRO_LAST.Y > 0)
				{
					Motor_Run(Motor_1, 0.1);
					Motor_Run(Motor_3, 1);
					delay_ms(100);
		
				}				
			}
			else if(Angle_Last.pit < -2 && Angle_Last.pit > -8)
			{
				if(MPU6050_GYRO_LAST.Y < 0)
				{
					Motor_Run(Motor_3, 0.1);
					Motor_Run(Motor_1, 1);
					delay_ms(100);
			
				}
			}
//---------------------------------------------------------------------------//
	
			if(Angle_Last.rol > 2 && Angle_Last.rol < 8)						
			{
				if(MPU6050_GYRO_LAST.X < 0)
				{
					Motor_Run(Motor_2, 0.1);
					Motor_Run(Motor_4, 1);
					delay_ms(100);
	
				}				
			}
			else if(Angle_Last.rol > -8 && Angle_Last.rol < -2)
			{
				if(MPU6050_GYRO_LAST.X > 0)
				{
					Motor_Run(Motor_4, 0.1);
					Motor_Run(Motor_2, 1);
					delay_ms(100);
				}
			}		
		}
	}
}

void Circle_CC(float GlobalAngle)
{
	float AngleRight_X = 0, AngleLift_X = 0, AngleAver_X = 0;
	float AngleRight_Y = 0, AngleLift_Y = 0, AngleAver_Y = 0;
	
	float SpeedAver_Y = 0, SpeedAver_X = 0;

//==========================================//
// 摆杆初始起摆
	Motor_Run(Motor_1, 1);
	Motor_Run(Motor_2, 1);
	Motor_Run(Motor_3, 0);
	delay_ms(1000);
//	Motor_Run(Motor_2, 0);
//	Motor_Run(Motor_1, 0);
//	Motor_Run(Motor_3, 1);
//	delay_ms(800);
//=========================================//

	Motor_Run(Motor_1, 0.1);
	Motor_Run(Motor_2, 0.1);
	Motor_Run(Motor_3, 0.1);
	Motor_Run(Motor_4, 0.1);
	
	PID_Global = GlobalAngle;
	
	while(1)
	{
		if(timerFlag == 1)	// 定时器采样标志位
		{	
			timerFlag = 0;
			
//-----------------------------------------------------------------------------//
			
			if(Angle_Last.pit > 2 && Angle_Last.pit < 4 )					
			{
				if(MPU6050_GYRO_LAST.Y > 0)
				{
					if(SpeedAver_Y >= 0)
					{
						Motor_Run(Motor_1, 0.1);
						Motor_Run(Motor_3, SpeedAver_Y);
						delay_ms(50);
					}
					else
					{
						Motor_Run(Motor_3, 0.1);
						Motor_Run(Motor_1, -1*SpeedAver_Y);
						delay_ms(50);
					}
				}			
			}
			else if(Angle_Last.pit < -2 && Angle_Last.pit > -4 )
			{
				if(MPU6050_GYRO_LAST.Y < 0)
				{
					if(SpeedAver_Y>=0)
					{
						Motor_Run(Motor_3, 0.1);
						Motor_Run(Motor_1, SpeedAver_Y);
						delay_ms(50);
					}
					else 
					{
						Motor_Run(Motor_1, 0.1);
						Motor_Run(Motor_3, -1*SpeedAver_Y);
						delay_ms(50);
					}
				}
			}
			
//---------------------------------------------------------------------//			
			if(Angle_Last.rol > 2 && Angle_Last.rol < 4 )					
			{
				if(MPU6050_GYRO_LAST.X < 0)
				{
					if(SpeedAver_Y >= 0)
					{
						Motor_Run(Motor_2, 0.1);
						Motor_Run(Motor_4, SpeedAver_X);
						delay_ms(50);
					}
					else
					{
						Motor_Run(Motor_4, 0.1);
						Motor_Run(Motor_2, -1*SpeedAver_X);
						delay_ms(50);
					}
				}			
			}
			else if(Angle_Last.rol < -2 && Angle_Last.rol > -4 )
			{
				if(MPU6050_GYRO_LAST.X > 0)
				{
					if(SpeedAver_Y >= 0)
					{
						Motor_Run(Motor_4, 0.1);
						Motor_Run(Motor_2, SpeedAver_X);
						delay_ms(50);
					}
					else 
					{
						Motor_Run(Motor_2, 0.1);
						Motor_Run(Motor_4, -1*SpeedAver_X);
						delay_ms(50);
					}
				}
			}

//===============================================================================//
// PID调节部分
			
			if(Angle_flag)
			{
				Angle_flag = 0;
	
				AngleRight_Y = AngleMax_Y;
				AngleLift_Y = AngleMin_Y;
				
				AngleRight_X = AngleMax_X;
				AngleLift_X = AngleMin_X;
//-------------------------------------------------------------------//					
				OLED_Shownumf(10, 2, 1, AngleMax_Y);
				OLED_Shownumf(10, 4, 1, AngleMax_X);
//-------------------------------------------------------------------//				
				AngleMax_Y = 0;
				AngleMin_Y = 0;
				
				AngleMax_X = 0;
				AngleMin_X = 0;
				
				AngleAver_Y = (AngleRight_Y + AngleLift_Y)/2.0;
				AngleAver_X = (AngleRight_X + AngleLift_X)/2.0;
				
				SpeedAver_Y = PID_Regulator(AngleRight_Y);		// 目前PID用的同一个，调试的时候分成两组单独调节参数
				
				SpeedAver_X = PID_Regulator(AngleRight_X);
				
				if(SpeedAver_Y > 1) SpeedAver_Y = 1;
				else if(SpeedAver_Y < -1) SpeedAver_Y = -1;	

				if(SpeedAver_X > 1) SpeedAver_X = 1;
				else if(SpeedAver_X < -1) SpeedAver_X = -1;	
			}
		}
	}
}



void StopPendulum(void)
{
	while(1)
	{
		if(timerFlag == 1)
		{
			timerFlag = 0;
			
//--------------------------------------------------------------------------------------//
			
			if(Angle_Last.pit > 2 && Angle_Last.pit < 8)						
			{
				if(MPU6050_GYRO_LAST.Y > 0)
				{
					Motor_Run(Motor_1, 1);
					Motor_Run(Motor_3, 0.1);
					delay_ms(100);
		
				}				
			}
			else if(Angle_Last.pit < -2 && Angle_Last.pit > -8)
			{
				if(MPU6050_GYRO_LAST.Y < 0)
				{
					Motor_Run(Motor_3, 1);
					Motor_Run(Motor_1, 0.1);
					delay_ms(100);
			
				}
			}
//---------------------------------------------------------------------------//
	
			if(Angle_Last.rol > 2 && Angle_Last.rol < 8)						
			{
				if(MPU6050_GYRO_LAST.X < 0)
				{
					Motor_Run(Motor_2, 1);
					Motor_Run(Motor_4, 0.1);
					delay_ms(100);
	
				}				
			}
			else if(Angle_Last.rol > -8 && Angle_Last.rol < -2)
			{
				if(MPU6050_GYRO_LAST.X > 0)
				{
					Motor_Run(Motor_4, 1);
					Motor_Run(Motor_2, 0.1);
					delay_ms(100);
				}
			}		
		}
	}	
}