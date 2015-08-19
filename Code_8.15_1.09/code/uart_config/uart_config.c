#include "stm32f10x.h"

uint8_t RxBuffer[20];
uint8_t RxCounter = 0, Flag_Receive = 0;

void GPIO_Configuration_UART(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOA, GPIOx and AFIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	/* Enable USART2 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

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

	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);
}

void NVIC_Configuration_UART(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
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
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == 0);
		USART_SendData(USART3, *tp);
		tp++;
	}
}
void USART3_IRQHandler(void)
{
	uint8_t tmp;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		tmp = USART_ReceiveData(USART3);
		
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
