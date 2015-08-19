#ifndef __motor_h__
#define __motor_h__

//==========================================================================================//
typedef struct
{
	int8_t	 		Direction;							// Direction = 1/-1/0 �����ǰת������, 1-->����  -1-->����  0-->��ת
	float 			SpeedSet;								// SpeedSet = 0~1 ����趨ת��, 0% -- 100%
	float				SpeedMeasured;					// SpeedMeasured ���ݱ����������źż�����ʵ���ٶ�ֵ
	float				SpeedMeasuredstr[25];
	int32_t			CodeCount;							// Code �����������źż�����
	uint8_t			IS_L; 									// IS_L = 1 or 0 ������⣬1-->������   0-->����
	uint8_t			IS_R;										// IS_R = 1 or 0 ������⣬1-->������   0-->����
	uint16_t		now_angle;							// now_angle �����ǰ�Ƕ�
	uint16_t		last_angle;							// last_angle�����һ������ڽǶ�
	
}Motor;

//---------------------------------------------------------------//

#define MAXCOUNT	 			1000		// �����������������������ֵ��ֵ
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
void AnglePendulum_CC(float Angle);  // Angle����Ϊ ��x��������Ϊ��㰴����ʱ�뷽��ת���ĽǶ�
void StopPendulum(void);
//==========================================================================================//

#endif
