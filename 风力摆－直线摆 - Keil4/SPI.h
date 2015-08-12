#ifndef __SPI_H
#define __SPI_H

#include <stm32f10x.h>

#define SPI					SPI2
#define SPI_NSS_H()	GPIOB->BSRR = GPIO_Pin_12
#define SPI_NSS_L()	GPIOB->BRR = GPIO_Pin_12

void SPI_Configuration(void);
uint32_t SPI_ReadWrite(uint32_t ui32Data);

#endif // __SPI_H
