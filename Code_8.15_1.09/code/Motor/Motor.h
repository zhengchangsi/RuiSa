#ifndef __motor_h__
#define __motor_h__

//==========================================================================================//
typedef struct
{
	int8_t	 		Direction;							// Direction = 1/-1/0 电机当前转动方向, 1-->正向  -1-->反向  0-->不转
	float 			SpeedSet;								// SpeedSet = 0~1 电机设定转速, 0% -- 100%
	float				SpeedMeasured;					// SpeedMeasured 根据编码器反馈信号计算电机实际速度值
	float				SpeedMeasuredstr[25];
	int32_t			CodeCount;							// Code 编码器反馈信号计数器
	uint8_t			IS_L; 									// IS_L = 1 or 0 电流检测，1-->非正常   0-->正常
	uint8_t			IS_R;										// IS_R = 1 or 0 电流检测，1-->非正常   0-->正常
	uint16_t		now_angle;							// now_angle 电机当前角度
	uint16_t		last_angle;							// last_angle电机上一检测周期角度
	
}Motor;

//---------------------------------------------------------------//

#define MAXCOUNT	 			1000		// 两个检测周期内允许的最大码值差值
#define MAXDUTY	 	 			0.8
#define MINDUTY		 			0.2
#define STOP_DUTYRANGE	0.1

#define		Motor_1		1
#define 	Motor_2		2
#define 	Motor_3		3
#define 	Motor_4		4

//---------------------------------------------------------------//

#define		forward		1
#define		back			-1
#define   stop			0

//---------------------------------------------------------------//


extern Motor MotorA;
extern Motor MotorB;
extern Motor MotorC;
extern Motor MotorD;

//==========================================================================================//

void GPIO_Counfiguration_Motor(void);
void Motor_Run(uint8_t Motorx, float speed);


//void SimplePendulum(void);
void AnglePendulum(void);
void Circle(void);
void SimplePendulum_X(void);
void SimplePendulum_Y(void);

void max_p_ang_Y(float *mz, float *mf);
void max_p_ang_X(float *mz, float *mf);
	
void SimplePendulum_Y_CC(float GlobalAngle);
void SimplePendulum_X_CC(float GlobalAngle);
void Circle_CC(float GlobalAngle);
void AnglePendulum_CC(float Angle);  // Angle定义为 以x轴正方向为起点按照逆时针方向转动的角度
void StopPendulum(void);
//==========================================================================================//

#endif
