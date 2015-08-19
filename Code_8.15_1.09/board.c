#include "stm32f10x.h"
#include "board.h"
#include "pwm.h"
#include "Motor.h"
#include "delay.h"
#include "uart_config.h"
#include "timer.h"

void GPIO_Counfiguration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/*******************************************************************/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		
/*******************************************************************/

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
void Board_Init(void)
{
	GPIO_Counfiguration();
	
//-----------------------------------//	
}
