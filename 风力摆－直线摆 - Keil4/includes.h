#ifndef __INCLUDES_H_
#define __INCLUDES_H_

#include <stm32f10x.h>
#include <stdint.h>
#include <math.h>
//#include "MPU9150.h"
#include "IQmath/IQmathLib.h"
#include "mpu6050.h"
//#include "oled.h"
#include "timer.h"
#include "misc.h"
#include "enconder.h"
#include "led.h"
#include "control.h"
//
#include "adc.h"
#include "IMU.h"



#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))


typedef struct{
				float SetVal;
				float ActualVal;
				float err;
				float err_last;
				float Kp,Ki,Kd;
				float voltage;			
				float integral;
}_PID;


_PID  PID_angular_velocity;   //
_PID  PID_angle_pose;         //
_PID  PID_pose;               //

u8 CAR_Status;
s16 DV;
s16 LIFT,RIGHT;

float BAT;

u8 data_to_send[0xff];











#endif

