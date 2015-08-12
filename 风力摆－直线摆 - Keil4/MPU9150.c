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

	if (I2C_ByteRead(MPU9150_SLAVE_ADDR, 0x75) != (MPU9150_SLAVE_ADDR >> 1))	//���MPU9150
	{
		while (1)	//DEBUG
		{
		}
	}

	/*I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x6B, 0x80);														//����MPU9150 ��λ
	while (I2C_ByteRead(MPU9150_SLAVE_ADDR, 0x6B) & 0x80)											//�ȴ�MPU9150 ��λ���
	{
	}*/

	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x6B, 0x01);														//����MPU9150 �˳�˯��ģʽ������PLLΪX axis

	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x6A, 0x00);														//����MPU9150 �ر�I2C��ģʽ
	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x37, 0x02);														//����MPU9150 ����I2C��·

	I2C_BufferRead(HMC5883L_SLAVE_ADDR, 0x0A, &ui8Buffer[0], 3);							//���HMC5883L
	if (ui8Buffer[0] != 'H' || ui8Buffer[1] != '4' || ui8Buffer[2] != '3')
	{
		while (1)	//DEBUG
		{
		}
	}

	ui8Buffer[0] = 0x70;																											//����HMC5883L ƽ��8�Σ��������Ƶ��15Hz����������ģʽ
	ui8Buffer[1] = 0x20;																											//����HMC5883L ����1090
	ui8Buffer[2] = 0x01;																											//����HMC5883L ���β���ģʽ
	I2C_BufferWrite(HMC5883L_SLAVE_ADDR, 0x00, &ui8Buffer[0], 3);

	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x37, 0x00);														//����MPU9150 �ر�I2C��·
	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x6A, 0x20);														//����MPU9150 ����I2C��ģʽ

	I2C_ByteWrite(MPU9150_SLAVE_ADDR, 0x67, 0x11);														//����MPU9150 ����SLV4_DLV������SLV0_DLV

	ui8Buffer[0] = 0x4D;																											//����MPU9150 �ȴ���������EXT_SENS_DATA������I2C��ģʽ400KHz
	ui8Buffer[1] = 0x80 | (HMC5883L_SLAVE_ADDR >> 1);													//����MPU9150 ���ö�HMC5883L
	ui8Buffer[2] = 0x03;																											//����MPU9150 ����HMC5883L��ʼ�Ĵ���
	ui8Buffer[3] = 0x80 | 0x06;																								//����MPU9150 ����SLV0�����ö��Ĵ�����Ŀ
	I2C_BufferWrite(MPU9150_SLAVE_ADDR, 0x24, &ui8Buffer[0], 4);

	ui8Buffer[0] = (HMC5883L_SLAVE_ADDR >> 1);																//����MPU9150 ����дHMC5883L
	ui8Buffer[1] = 0x02;																											//����MPU9150 ����HMC5883L��ʼ�Ĵ���
	ui8Buffer[2] = 0x01;																											//����MPU9150 ����д����
	ui8Buffer[3] = 0x80 | 0x04;																								//����MPU9150 ����SLV4�����÷�ƵΪ4 + 1
	I2C_BufferWrite(MPU9150_SLAVE_ADDR, 0x31, &ui8Buffer[0], 4);

	ui8Buffer[0] = 0x01;																											//����MPU9150 ���÷�ƵΪ1 + 1
	ui8Buffer[1] = 0x02;																											//����MPU9150 ���ô���Ϊ94Hz 98Hz��������1KHz
	ui8Buffer[2] = 0x00;																											//����MPU9150 ���÷�ΧΪ250��/s
	ui8Buffer[3] = 0x01;																											//����MPU9150 ���÷�ΧΪ2g�����ø�ͨ�˲���Ϊ5Hz
	I2C_BufferWrite(MPU9150_SLAVE_ADDR, 0x19, &ui8Buffer[0], 4);

	I2C_DMALastTransferCmd(I2C, ENABLE);
	I2C_DMACmd(I2C, ENABLE);
}

void DMA1_Channel7_IRQHandler(void)
{
	if (DMA1->ISR & DMA_ISR_TCIF7)
	{
		I2C->CR1 |= 0x200;	//ֹͣ��������

		DMA1_Channel7->CCR &= (uint16_t)(~DMA_CCR1_EN);
		DMA1->IFCR = DMA_IFCR_CGIF7;
	}
}

void MPU9150_BufferRead(uint8_t ui8ReadAddr, uint8_t *pui8Buffer, uint32_t ui32NumByteToRead)
{
	while (I2C->SR2 & 0x02);

	I2C->CR1 |= 0x100;	//��ʼ��������
	while (!(I2C->SR1 & 0x01));	//�ȴ���ʼ��������

	I2C->DR = MPU9150_SLAVE_ADDR & 0xfe;	//���ʹ�������ַ
	while ((I2C->SR1 & 0x82) != 0x82);	//�ȴ���ַ���������ͼĴ���Ϊ��
	(void)I2C->SR2;	//�����־λ

	I2C->DR = ui8ReadAddr;	//���Ͷ���ַ
	while ((I2C->SR1 & 0x84) != 0x84);	//�ȴ����ͼĴ����գ���λ�Ĵ�����

	I2C->CR1 |= 0x100;	//��ʼ��������
	while (!(I2C->SR1 & 0x01));	//�ȴ���ʼ��������

	I2C->DR = MPU9150_SLAVE_ADDR | 0x01;	//���ʹ�������ַ
	while (!(I2C->SR1 & 0x02));	//�ȴ���ַ����
	(void)I2C->SR2;	//�����־λ

	DMA1_Channel7->CMAR = (uint32_t)pui8Buffer;
	DMA1_Channel7->CNDTR = ui32NumByteToRead;
	DMA1_Channel7->CCR |= DMA_CCR1_EN;
}
