#include <stm32f10x.h>
#include "I2C.h"
#include "HW_MPU9150.h"
#include "MPU9150.h"

void MPU9150_Init(void)
{
	uint8_t ui8Buffer[8];

	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_DeInit(DMA1_Channel7);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&I2C->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;												/* This parameter will be configured durig communication */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 0xFFFF;																/* This parameter will be configured durig communication */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

	I2C_Configuration();

	if (I2C_ByteRead(MPU9150_SLAVE_ADDR, 0x75) != (MPU9150_SLAVE_ADDR >> 1))	//检测MPU9150
	{
		while (1)	//DEBUG
		{
		}
	}

	/*I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x6B, 0x80);														//设置MPU9150 复位
	while (I2C_ByteRead(MPU9150_SLAVE_ADDR, 0x6B) & 0x80)											//等待MPU9150 复位完成
	{
	}*/

	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x6B, 0x01);														//设置MPU9150 退出睡眠模式，设置PLL为X axis

	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x6A, 0x00);														//设置MPU9150 关闭I2C主模式
	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x37, 0x02);														//设置MPU9150 开启I2C旁路

	I2C_BufferRead(HMC5883L_SLAVE_ADDR, 0x0A, &ui8Buffer[0], 3);							//检测HMC5883L
	if (ui8Buffer[0] != 'H' || ui8Buffer[1] != '4' || ui8Buffer[2] != '3')
	{
		while (1)	//DEBUG
		{
		}
	}

	ui8Buffer[0] = 0x70;																											//设置HMC5883L 平均8次，数据输出频率15Hz，正常测量模式
	ui8Buffer[1] = 0x20;																											//设置HMC5883L 增益1090
	ui8Buffer[2] = 0x01;																											//设置HMC5883L 单次测量模式
	I2C_BufferWrite(HMC5883L_SLAVE_ADDR, 0x00, &ui8Buffer[0], 3);

	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x37, 0x00);														//设置MPU9150 关闭I2C旁路
	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x6A, 0x20);														//设置MPU9150 开启I2C主模式

	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x67, 0x11);														//设置MPU9150 开启SLV4_DLV，开启SLV0_DLV

	ui8Buffer[0] = 0x4D;																											//设置MPU9150 等待数据载入EXT_SENS_DATA，设置I2C主模式400KHz
	ui8Buffer[1] = 0x80 | (HMC5883L_SLAVE_ADDR >> 1);													//设置MPU9150 设置读HMC5883L
	ui8Buffer[2] = 0x03;																											//设置MPU9150 设置HMC5883L起始寄存器
	ui8Buffer[3] = 0x80 | 0x06;																								//设置MPU9150 开启SLV0，设置读寄存器数目
	I2C_BufferWrite(MPU9150_SLAVE_ADDR, 0x24, &ui8Buffer[0], 4);

	ui8Buffer[0] = (HMC5883L_SLAVE_ADDR >> 1);																//设置MPU9150 设置写HMC5883L
	ui8Buffer[1] = 0x02;																											//设置MPU9150 设置HMC5883L起始寄存器
	ui8Buffer[2] = 0x01;																											//设置MPU9150 设置写数据
	ui8Buffer[3] = 0x80 | 0x04;																								//设置MPU9150 开启SLV4，设置分频为4 + 1
	I2C_BufferWrite(MPU9150_SLAVE_ADDR, 0x31, &ui8Buffer[0], 4);

	ui8Buffer[0] = 0x01;																											//设置MPU9150 设置分频为1 + 1
	ui8Buffer[1] = 0x02;																											//设置MPU9150 设置带宽为94Hz 98Hz，采样率1KHz
	ui8Buffer[2] = 0x00;																											//设置MPU9150 设置范围为250度/s
	ui8Buffer[3] = 0x01;																											//设置MPU9150 设置范围为2g，设置高通滤波器为5Hz
	I2C_BufferWrite(MPU9150_SLAVE_ADDR, 0x19, &ui8Buffer[0], 4);

	I2C_DMALastTransferCmd(I2C, ENABLE);
	I2C_DMACmd(I2C, ENABLE);
}

void DMA1_Channel7_IRQHandler(void)
{
	if (DMA1->ISR & DMA_ISR_TCIF7)
	{
		I2C->CR1 |= 0x200;	//停止条件建立

		DMA1_Channel7->CCR &= (uint16_t)(~DMA_CCR1_EN);
		DMA1->IFCR = DMA_IFCR_CGIF7;
	}
}

void MPU9150_BufferRead(uint8_t ui8ReadAddr, uint8_t *pui8Buffer, uint32_t ui32NumByteToRead)
{
	while (I2C->SR2 & 0x02);

	I2C->CR1 |= 0x100;	//起始条件建立
	while (!(I2C->SR1 & 0x01));	//等待起始条件建立

	I2C->DR = MPU9150_SLAVE_ADDR & 0xfe;	//发送从器件地址
	while ((I2C->SR1 & 0x82) != 0x82);	//等待地址建立，发送寄存器为空
	(void)I2C->SR2;	//清除标志位

	I2C->DR = ui8ReadAddr;	//发送读地址
	while ((I2C->SR1 & 0x84) != 0x84);	//等待发送寄存器空，移位寄存器空

	I2C->CR1 |= 0x100;	//起始条件建立
	while (!(I2C->SR1 & 0x01));	//等待起始条件建立

	I2C->DR = MPU9150_SLAVE_ADDR | 0x01;	//发送从器件地址
	while (!(I2C->SR1 & 0x02));	//等待地址建立
	(void)I2C->SR2;	//清除标志位

	DMA1_Channel7->CMAR = (uint32_t)pui8Buffer;
	DMA1_Channel7->CNDTR = ui32NumByteToRead;
	DMA1_Channel7->CCR |= DMA_CCR1_EN;
}
