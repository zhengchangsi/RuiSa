#ifndef CONTROL_H_
#define CONTROL_H_
#include "sys.h"

void Control_gpio_init(void);
void Direction_control(u8 dir, u8 motor_num);
void Motor_A_contro(uint16_t Motor_A_speed,u8 Motor_A_dir);
void Motor_B_contro(uint16_t Motor_B_speed,u8 Motor_B_dir);

void Motor_Speed_Ctrl(s16 Motor_speed,u8 Motor_Num);


#define MOTOR_NUM_A    0X0F
#define MOTOR_NUM_B		 0XFF
#define MOTOR_NUM_C    0X1F
#define MOTOR_NUM_D    0X2F

#define MOTOR_DIR_FRONT	0x0F
#define MOTOR_DIR_BACK	0xFF
#define MOTOR_DIR_STOP	0x88

#define CAR_DIR_UP	  0x05
#define CAR_DIR_DOWN	0x06
#define CAR_DIR_RIGHT	0x07
#define CAR_DIR_LIFT	0x08
#define CAR_DIR_STOP	0x09


typedef struct{
				float SetVal;
				float ActualVal;
				float err;
				float err_last;
				float Kp,Ki,Kd;
				float voltage;			
				float integral;
}_PID;





#endif


