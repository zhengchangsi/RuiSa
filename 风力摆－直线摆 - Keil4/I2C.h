#ifndef __I2C_H
#define __I2C_H

#include <stm32f10x.h>

#define I2C							I2C1
#define I2C_SLAVE_ADDR	0x32
#define I2C_SPEED				400000
#define I2C_SCL_PIN			GPIO_Pin_8
#define I2C_SDA_PIN			GPIO_Pin_9

void I2C_Configuration(void);
void I2C_ByteWrite(uint8_t ui8SlaveAddr, uint8_t ui8WriteAddr, uint8_t ui8Data);
void I2C_BufferWrite(uint8_t ui8SlaveAddr, uint8_t ui8WriteAddr, uint8_t *pui8Buffer, uint32_t ui32NumByteToWrite);
uint8_t I2C_ByteRead(uint8_t ui8SlaveAddr, uint8_t ui8ReadAddr);
void I2C_BufferRead(uint8_t ui8SlaveAddr, uint8_t ui8ReadAddr, uint8_t *pui8Buffer, uint32_t ui32NumByteToRead);

#endif // __I2C_H
