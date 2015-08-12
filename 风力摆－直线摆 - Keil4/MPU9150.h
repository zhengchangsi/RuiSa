#ifndef __MPU9150_H
#define __MPU9150_H

#include <stm32f10x.h>

#define MPU9150_SLAVE_ADDR	0xD0
#define HMC5883L_SLAVE_ADDR	0x3C

struct sSensor
{
	int16_t acc[3];
	int16_t tem;
	int16_t	gyr[3];
	int16_t mag[3];
};

void MPU9150_Init(void);
void MPU9150_BufferRead(uint8_t ui8ReadAddr, uint8_t *pui8Buffer, uint32_t ui32NumByteToRead);

#endif // __MPU9150_H
