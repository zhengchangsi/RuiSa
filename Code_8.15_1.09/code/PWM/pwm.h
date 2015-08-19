#ifndef __pwm_h__
#define __pwm_h__


void PWM_Init(void);
void Timer_Configuration_PWM(void);
void GPIO_Configuration_PWM(void);
void PWM_Pulse_Change(uint8_t channel, uint16_t Pulse);

#define D_TIM1_period (uint16_t)(36000-1)  // 500 Hz

#endif
