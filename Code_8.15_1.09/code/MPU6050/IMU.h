#ifndef __IMU_H__
#define __IMU_H__

#include "mpu6050.h"

extern T_FLOAT_ANGLE Angle_Last;
//void Prepare_Data(T_int16_xyz *acc_in,T_int16_xyz *acc_out);// ACC deal with	fliter
void IMUupdate(S_INT16_XYZ *gyr, S_INT16_XYZ *acc, T_FLOAT_ANGLE *angle); 


//void MadgwickAHRSupdateIMU(T_int16_xyz *gyr, T_int16_xyz *acc, T_float_angle *angle);
//void Prints(int16_t dat);
//void Att_Deal(T_float_angle *angle,u8 *buf);
#endif

