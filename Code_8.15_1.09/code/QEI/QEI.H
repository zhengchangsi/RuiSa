#ifndef _QEI_H_
#define _QEI_H_

void QEI_Init(void);
void GPIO_Configuration_QEI(void);
void QEI_Configuration(void);
//void NVIC_Configuartion_TIM2(void);

#define EncoderPeriod 1393
//extern uint8_t TIM2_IT_flag;


#endif
