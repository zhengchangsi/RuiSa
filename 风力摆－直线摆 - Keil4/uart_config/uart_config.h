/********************************************************

File Name: uart_config.h
Date:      2013-04-18 

˵����Nothing.
********************************************************/

#ifndef __uart_config_h_
#define __uart_config_h_
//--------------------------------------------------//
#include "stm32f10x.h"
#include "control.h"





#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))







extern uint8_t RxBuffer[20];
extern uint8_t RxCounter, Flag_Receive;

//--------------------------------------------------//

void UART_Init(uint32_t Baud_Rate);
											// UART��ʼ�� Baud_Rate ������
//--------------------------------------------------//
void UART_SendStr(uint8_t *tp);
											// �ַ������ͺ���
//--------------------------------------------------//

void Bluetooth_reseive(_PID* PID1,_PID* PID2,u8* CAR_Status);







#endif
