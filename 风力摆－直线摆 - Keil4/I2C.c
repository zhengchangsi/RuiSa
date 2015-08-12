#include <stm32f10x.h>
#include "I2C.h"

#if defined(ewarm) || defined(DOXYGEN)
void I2CDelay(uint32_t ui32Count)
{
	__asm("    subs    r0, #1\n"
	"    bne.n   I2CDelay\n"
		"    bx      lr");
}
#endif
#if defined(codered) || defined(gcc) || defined(sourcerygxx)
void __attribute__((naked)) I2CDelay(uint32_t ui32Count)
{
	__asm("    subs    r0, #1\n"
	"    bne     I2CDelay\n"
		"    bx      lr");
}
#endif
#if defined(rvmdk) || defined(__ARMCC_VERSION)
__asm void I2CDelay(uint32_t ui32Count)
{
	subs    r0, #1;
	bne     I2CDelay;
	bx      lr;
}
#endif

void I2C_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB, I2C_SCL_PIN);
	I2CDelay(SystemCoreClock / 3 / 100000);

	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB, I2C_SDA_PIN);
	I2CDelay(SystemCoreClock / 3 / 100000);

	GPIO_SetBits(GPIOB, I2C_SCL_PIN);
	I2CDelay(SystemCoreClock / 3 / 100000);

	GPIO_SetBits(GPIOB, I2C_SDA_PIN);
	I2CDelay(SystemCoreClock / 3 / 100000);

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDR;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
	I2C_Init(I2C, &I2C_InitStructure);

	I2C_Cmd(I2C, ENABLE);
	
	if (I2C->SR2 & 0x02)	//���I2C
	{
		while (1)	//DEBUG
		{
		}
	}
}

void I2C_ByteWrite(uint8_t ui8SlaveAddr, uint8_t ui8WriteAddr, uint8_t ui8Data)
{
	while (I2C->SR2 & 0x02);

	I2C->CR1 |= 0x100;	//��ʼ��������
	while (!(I2C->SR1 & 0x01));	//�ȴ���ʼ��������

	I2C->DR = ui8SlaveAddr & 0xfe;	//���ʹ�������ַ
	while ((I2C->SR1 & 0x82) != 0x82);	//�ȴ���ַ���������ͼĴ���Ϊ��
	(void)I2C->SR2;	//�����־λ

	I2C->DR = ui8WriteAddr;	//����д��ַ
	while (!(I2C->SR1 & 0x80));	//�ȴ����ͼĴ�����

	I2C->DR = ui8Data;	//�����ֽ�
	while ((I2C->SR1 & 0x84) != 0x84);	//�ȴ����ͼĴ����գ���λ�Ĵ�����

	I2C->CR1 |= 0x200;	//ֹͣ��������
}

void I2C_BufferWrite(uint8_t ui8SlaveAddr, uint8_t ui8WriteAddr, uint8_t *pui8Buffer, uint32_t ui32NumByteToWrite)
{
	while (I2C->SR2 & 0x02);

	I2C->CR1 |= 0x100;	//��ʼ��������
	while (!(I2C->SR1 & 0x01));	//�ȴ���ʼ��������

	I2C->DR = ui8SlaveAddr & 0xfe;	//���ʹ�������ַ
	while ((I2C->SR1 & 0x82) != 0x82);	//�ȴ���ַ���������ͼĴ���Ϊ��
	(void)I2C->SR2;	//�����־λ

	I2C->DR = ui8WriteAddr;	//����д��ַ
	while (ui32NumByteToWrite--)
	{
		while (!(I2C->SR1 & 0x80));	//�ȴ����ͼĴ�����
		I2C->DR = *pui8Buffer;	//�����ֽ�
		pui8Buffer++;
	}
	while ((I2C->SR1 & 0x84) != 0x84);	//�ȴ����ͼĴ����գ���λ�Ĵ�����

	I2C->CR1 |= 0x200;	//ֹͣ��������
}

uint8_t I2C_ByteRead(uint8_t ui8SlaveAddr, uint8_t ui8ReadAddr)
{
	uint8_t ui8Data;

	while (I2C->SR2 & 0x02);

	I2C->CR1 |= 0x100;	//��ʼ��������
	while (!(I2C->SR1 & 0x01));	//�ȴ���ʼ��������

	I2C->DR = ui8SlaveAddr & 0xfe;	//���ʹ�������ַ
	while ((I2C->SR1 & 0x82) != 0x82);	//�ȴ���ַ���������ͼĴ���Ϊ��
	(void)I2C->SR2;	//�����־λ

	I2C->DR = ui8ReadAddr;	//���Ͷ���ַ
	while ((I2C->SR1 & 0x84) != 0x84);	//�ȴ����ͼĴ����գ���λ�Ĵ�����

	I2C->CR1 |= 0x100;	//��ʼ��������
	while (!(I2C->SR1 & 0x01));	//�ȴ���ʼ��������

	I2C->DR = ui8SlaveAddr | 0x01;	//���ʹ�������ַ
	while (!(I2C->SR1 & 0x02));	//�ȴ���ַ����
	(void)I2C->SR2;	//�����־λ

	I2C->CR1 &= 0xfbff;	//Ӧ���ֹ
	I2C->CR1 |= 0x200;	//ֹͣ��������

	while (!(I2C->SR1 & 0x40));	//���ռĴ�����Ϊ��
	ui8Data = I2C->DR;

	I2C->CR1 |= 0x400;	//Ӧ��ʹ��

	return ui8Data;
}

void I2C_BufferRead(uint8_t ui8SlaveAddr, uint8_t ui8ReadAddr, uint8_t *pui8Buffer, uint32_t ui32NumByteToRead)
{
	while (I2C->SR2 & 0x02);

	I2C->CR1 |= 0x100;	//��ʼ��������
	while (!(I2C->SR1 & 0x01));	//�ȴ���ʼ��������

	I2C->DR = ui8SlaveAddr & 0xfe;	//���ʹ�������ַ
	while ((I2C->SR1 & 0x82) != 0x82);	//�ȴ���ַ���������ͼĴ���Ϊ��
	(void)I2C->SR2;	//�����־λ

	I2C->DR = ui8ReadAddr;	//���Ͷ���ַ
	while ((I2C->SR1 & 0x84) != 0x84);	//�ȴ����ͼĴ����գ���λ�Ĵ�����

	I2C->CR1 |= 0x100;	//��ʼ��������
	while (!(I2C->SR1 & 0x01));	//�ȴ���ʼ��������

	I2C->DR = ui8SlaveAddr | 0x01;	//���ʹ�������ַ
	while (!(I2C->SR1 & 0x02));	//�ȴ���ַ����
	(void)I2C->SR2;	//�����־λ

	while (ui32NumByteToRead)
	{
		if (ui32NumByteToRead == 1)
		{
			I2C->CR1 &= 0xfbff;	//Ӧ���ֹ
			I2C->CR1 |= 0x200;	//ֹͣ��������
		}

		while (!(I2C->SR1 & 0x40));	//���ռĴ�����Ϊ��
		*pui8Buffer = I2C->DR;
		pui8Buffer++;
		ui32NumByteToRead--;
	}

	I2C->CR1 |= 0x400;	//Ӧ��ʹ��
}
