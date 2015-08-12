#include "stm32f10x.h"
#include "control.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>



uint8_t RxBuffer[0xff];
uint8_t RxCounter = 0, Flag_Receive = 0;

void GPIO_Configuration_UART(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOA, GPIOx and AFIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	/* Enable USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void UART_Set(u32 BaR)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = BaR;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

void NVIC_Configuration_UART(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void UART_Init(uint32_t Baud_Rate)
{
	GPIO_Configuration_UART();
	UART_Set(Baud_Rate);
	NVIC_Configuration_UART();
}

void UART_SendStr(u8 *tp)
{
	while(*tp)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == 0);
		USART_SendData(USART1, *tp);
		tp++;
	}
}

void USART1_IRQHandler(void)
{
	uint8_t tmp=0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		tmp = USART_ReceiveData(USART1);
		
		if(tmp == '#') 
		{
			Flag_Receive = 1;
		}
		else
		{
			RxBuffer[RxCounter++] =  tmp;     
		}
		RxBuffer[RxCounter] = '\0';
	}
}


 u8 PID_SW_flag;
void Bluetooth_reseive(_PID* PID1,_PID* PID2,u8* CAR_Status)
{
		u8 i;
		u8 str_PID[0xff];
		
		float P,I,D;
	
		if(Flag_Receive)
		{ 
			Flag_Receive = 0;
			
			for(i = 0; i < RxCounter; i++){
				str_PID[i] = RxBuffer[i+1];
			}
			
			switch(RxBuffer[0]){
				case 'P':{
					sscanf((char*)str_PID,"%f", &P);
					if(PID_SW_flag == 1)PID1->Kp = P/50.0;
					else if(PID_SW_flag == 0)PID2->Kp = P/50.0;
					else ;
					break;
				}
				case 'I':{
					sscanf((char*)str_PID,"%f", &I);
					if(PID_SW_flag == 1)PID1->Ki = I/100.0;
					else if(PID_SW_flag == 0)PID2->Ki = I/5000.0;
					else ;
					break;
				}
				case 'D':{
					sscanf((char*)str_PID,"%f", &D);
					if(PID_SW_flag == 1)PID1->Kd = D/2;
					else if(PID_SW_flag == 0)PID2->Kd = D/2;
					else ;
					break;
				}
				case 'S':{
					PID_SW_flag = 1;
					break;
				}
				case 'W':{
					PID_SW_flag = 0;
					break;
				}
				case 'C':{
					switch(RxBuffer[1])
					{
						case 'U':{
							*CAR_Status = CAR_DIR_UP;
							break;
						}
						case 'L':{
							*CAR_Status = CAR_DIR_LIFT;
							break;
						}	
						case 'D':{
							*CAR_Status = CAR_DIR_DOWN;
							break;
						}						
						case 'R':{
							*CAR_Status = CAR_DIR_RIGHT;
						break;
						}
						case 'S':{
							*CAR_Status = CAR_DIR_STOP;
							break;
						}
					}
					break;
				}
				default:
					break;
			}		
			RxCounter = 0;
		}
	}

	
	

	
	




