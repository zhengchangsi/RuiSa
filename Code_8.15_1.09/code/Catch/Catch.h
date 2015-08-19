#ifndef __CATCH_H_
#define __CATCH_H_

//=====================================//

void GPIO_Configuration_Catch(void);
void Catch_Configuration(void);
void NVIC_Configuration_Catch(void);

//=====================================//


extern uint16_t TIM2_Cap1, TIM2_Cap2;
extern uint8_t TIM2Cap_flag;

//=====================================//



#endif
