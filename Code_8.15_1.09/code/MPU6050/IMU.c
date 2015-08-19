
#include "mpu6050.h"
#include "math.h"


#define Kp 1.6f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Kpn 0.2f
#define Ki 0.0009f                          // integral gain governs rate of convergence of gyroscope biases
//#define halfT 0.001065f                   // half the sample period???????
#define halfT 0.001500f  

#define Acc_G 	1.0f//0.0011963				//   加速度变成G 1000/8192*0.0098
#define Gyro_G 	0.0610351f				//   此参数对应陀螺2000度每秒，比例 °/s /lsb
#define Gyro_Gr	0.0010653f				// 	 此参数对应陀螺2000度每秒 , rad/s /lsb = 角速度标准单位 弧度每秒
//#define Gyro_Gr	0.0008653f				// 	 此参数对应陀螺2000度每秒 , rad/s /lsb = 角速度标准单位 弧度每秒


float 	AngleOffset_Rol=0,AngleOffset_Pit=0;

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // 表示估计方向的四元数元素
float exInt = 0, eyInt = 0, ezInt = 0;    // 缩放积分误差

T_FLOAT_ANGLE  Angle_Last;


/*************************************************************
函数名：IMUupdate
功  能：计算姿态
输  入：S_INT16_XYZ *gyr,    //最新读取的MPU6050的角速度结构体
	    S_INT16_XYZ *acc, 	 //最新读取的MPU6050的加速度结构体
	    T_FLOAT_ANGLE *angle //存放计算出的姿态 ROL YAW PITH
输  出：
**************************************************************/


void IMUupdate(S_INT16_XYZ *gyr, S_INT16_XYZ *acc, T_FLOAT_ANGLE *angle)
{
	float ax = (acc->X)*Acc_G,ay = (acc->Y)*Acc_G,az = (acc->Z)*Acc_G;
	float gx = gyr->X,gy = gyr->Y,gz = gyr->Z;
	float norm;
//  float hx, hy, hz, bx, bz;
	float vx, vy, vz;// wx, wy, wz;
	float ex, ey, ez;

  // ???????????
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
//  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
//  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
	
	if(ax*ay*az==0)
 		return;
		
	gx *= Gyro_Gr;	 //°/s 转化为 rad/s
	gy *= Gyro_Gr;
	gz *= Gyro_Gr;	

  norm = sqrt(ax*ax+ay*ay+az*az);
  ax = ax /norm;
  ay = ay /norm;
  az = az /norm;
 

  // estimated direction of gravity and flux (v and w)              ?????????/??
  vx = 2*(q1q3 - q0q2);												//????xyz???
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                           					 //???????????????
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki;								  //???????
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;

  // adjusted gyroscope measurements
  gx = gx + Kp*ex + exInt;					   							//???PI???????,???????
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;
  					   							//???gz????????????????,??????????????
  // integrate quaternion rate and normalise						   //????????
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // normalise quaternion
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;

	angle->yaw += gyr->Z*Gyro_G*2*halfT;
	angle->pit = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3 - AngleOffset_Pit + 4.3; // pitch
	angle->rol = -atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3 - AngleOffset_Rol - 3.7; // roll
}

