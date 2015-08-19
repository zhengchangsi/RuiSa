/********************************************************

File Name: uart_config.h
Date:      2013-04-18 

说明：Nothing.
********************************************************/

#ifndef __uart_config_h_
#define __uart_config_h_
//--------------------------------------------------//

extern uint8_t RxBuffer[20];
extern uint8_t RxCounter, Flag_Receive;

//--------------------------------------------------//

void UART_Init(uint32_t Baud_Rate);
											// UART初始化 Baud_Rate 波特率
//--------------------------------------------------//
void UART_SendStr(uint8_t *tp);
											// 字符串发送函数
//--------------------------------------------------//
#endif
