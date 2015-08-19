#ifndef __timer_h_
#define __timer_h_

																
#define	SamplePeriod	3000			// 采样周期    单位:us   
// 初始值：3ms

extern uint16_t timerCount;
extern uint16_t timerFlag;

void Timer3_Configuartion(void);
void NVIC_Configuartion_TIM3(void);
void Timer3_Init(void);

extern float angle_speed_last; 
extern uint8_t angle_speed_dir;

extern float AngleMax_X, AngleMin_X;
extern float AngleMax_Y, AngleMin_Y;
extern uint8_t Angle_flag;

#endif
