#include <stm32f10x.h>
#include "SPI.h"
#include "NRF24L01.h"

const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = { 0xE1, 0xE2, 0xE3, 0xE4, 0xE5 };	//���ص�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = { 0xE1, 0xE2, 0xE3, 0xE4, 0xE5 };	//���յ�ַ

void NRF24L01_Init(uint8_t mode, uint8_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	SPI_Configuration();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	//CE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	NRF24L01_CE_L();

	if (NRF24L01_Check() != 1)
	{
		while (1)	//DEBUG
		{
		}
	}

	NRF_Write_Buf(W_REGISTER + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);	//дRX�ڵ��ַ
	NRF_Write_Buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);		//дTX�ڵ��ַ
	NRF_Write_Reg(W_REGISTER + EN_AA, 0x01);													//ʹ��ͨ��0���Զ�Ӧ��
	NRF_Write_Reg(W_REGISTER + EN_RXADDR, 0x01);											//ʹ��ͨ��0�Ľ��յ�ַ 
	NRF_Write_Reg(W_REGISTER + SETUP_RETR, 0x1a);											//�����Զ��ط����ʱ��:500us;����Զ��ط�����:10�� 
	NRF_Write_Reg(W_REGISTER + RF_CH, channel);												//����RFͨ��ΪCHANAL
	NRF_Write_Reg(W_REGISTER + RF_SETUP, 0x0f);												//����TX�������,0db����,2Mbps,���������濪��
	//NRF_Write_Reg(W_REGISTER + RF_SETUP,0x07);											//����TX�������,0db����,1Mbps,���������濪��

	if (mode == MODE_RX)				//RX
	{
		NRF_Write_Reg(W_REGISTER + RX_PW_P0, RX_PLOAD_WIDTH);	//ѡ��ͨ��0����Ч���ݿ�� 
		NRF_Write_Reg(W_REGISTER + CONFIG, 0x0f);					//IRQ�շ�����жϿ���,16λCRC,������
	}
	else if (mode == MODE_TX)		//TX
	{
		NRF_Write_Reg(W_REGISTER + RX_PW_P0, RX_PLOAD_WIDTH);	//ѡ��ͨ��0����Ч���ݿ�� 
		NRF_Write_Reg(W_REGISTER + CONFIG, 0x0e);					//IRQ�շ�����жϿ���,16λCRC,������
	}
	else if (mode == MODE_RX2)	//RX2
	{
		NRF_Write_Reg(FLUSH_TX, 0xff);
		NRF_Write_Reg(FLUSH_RX, 0xff);
		NRF_Write_Reg(W_REGISTER + CONFIG, 0x0f);					//IRQ�շ�����жϿ���,16λCRC,������

		SPI_ReadWrite(0x50);
		SPI_ReadWrite(0x73);
		NRF_Write_Reg(W_REGISTER + DYNPD, 0x01);
		NRF_Write_Reg(W_REGISTER + FEATURE, 0x06);
	}
	else												//TX2
	{
		NRF_Write_Reg(W_REGISTER + CONFIG, 0x0e);					//IRQ�շ�����жϿ���,16λCRC,������
		NRF_Write_Reg(FLUSH_TX, 0xff);
		NRF_Write_Reg(FLUSH_RX, 0xff);

		SPI_ReadWrite(0x50);
		SPI_ReadWrite(0x73);
		NRF_Write_Reg(W_REGISTER + DYNPD, 0x01);
		NRF_Write_Reg(W_REGISTER + FEATURE, 0x06);
	}

	NRF24L01_CE_H();
}

uint32_t NRF24L01_Check(void)
{
	uint8_t i;
	uint8_t buffer[TX_ADR_WIDTH];

	NRF_Write_Buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	//д��5���ֽڵĵ�ַ
	NRF_Read_Buf(TX_ADDR, buffer, TX_ADR_WIDTH);	//����д��ĵ�ַ

	for (i = 0; i < 5; i++)	//�Ƚ�
	{
		if (buffer[i] != TX_ADDRESS[i])
			break;
	}
	if (i >= 5)
		return 1;	//MCU��NRF�ɹ����� 
	else
		return 0;		//MCU��NRF���������� 
}

/*
*****************************************************************
* д�Ĵ���
*****************************************************************
*/
uint8_t NRF_Write_Reg(uint8_t WriteAddr, uint8_t uData)
{
	uint8_t status;

	SPI_NSS_L();												/* ѡͨ���� */
	status = SPI_ReadWrite(WriteAddr);	/* д�Ĵ���д��ַ */
	SPI_ReadWrite(uData);		  				/* д���� */
	SPI_NSS_H();												/* ��ֹ������ */
	return status;
}
/*
*****************************************************************
* ���Ĵ���
*****************************************************************
*/
uint8_t NRF_Read_Reg(uint8_t ReadAddr)
{
	uint8_t uData;

	SPI_NSS_L();								/* ѡͨ���� */
	SPI_ReadWrite(ReadAddr);		/* д�Ĵ�������ַ */
	uData = SPI_ReadWrite(0);	/* ��ȡ�üĴ����������� */
	SPI_NSS_H();								/* ��ֹ������ */
	return uData;
}
/*
*****************************************************************
* д������
*****************************************************************
*/
uint8_t NRF_Write_Buf(uint8_t WriteAddr, const uint8_t* pBuffer, uint8_t NumByteToWrite)
{
	uint8_t status;

	SPI_NSS_L();												/* ѡͨ���� */
	status = SPI_ReadWrite(WriteAddr);	/* д�Ĵ�����ַ */
	while (NumByteToWrite--)
	{
		SPI_ReadWrite(*pBuffer);					/* д���� */
		pBuffer++;
	}
	SPI_NSS_H();												/* ��ֹ������ */
	return status;
}
/*
*****************************************************************
* ��������
*****************************************************************
*/
uint8_t NRF_Read_Buf(uint8_t ReadAddr, uint8_t* pBuffer, uint8_t NumByteToRead)
{
	uint8_t status;

	SPI_NSS_L();												/* ѡͨ���� */
	status = SPI_ReadWrite(ReadAddr);	/* д�Ĵ�����ַ */
	while (NumByteToRead--)
	{
		*pBuffer = SPI_ReadWrite(0);			/* ��ȡ�������� */
		pBuffer++;
	}
	SPI_NSS_H();												/* ��ֹ������ */
	return status;
}
/*
*****************************************************************
* д���ݰ�
*****************************************************************
*/
void NRF_TxPacket(const uint8_t* pBuffer, uint8_t NumByteToWrite)
{
	NRF24L01_CE_L();	//StandBy Iģʽ	
	NRF_Write_Buf(W_REGISTER + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	//װ�ؽ��ն˵�ַ�����ڽ���Ӧ��
	NRF_Write_Buf(W_TX_PLOAD, pBuffer, NumByteToWrite);	//װ������	
	NRF24L01_CE_H();	//�ø�CE���������ݷ���
}

void NRF_TxPacket_AP(const uint8_t* pBuffer, uint8_t NumByteToWrite)
{
	NRF24L01_CE_L();	//StandBy Iģʽ
	NRF_Write_Buf(W_ACK_PAYLOAD, pBuffer, NumByteToWrite);	//װ������
	NRF24L01_CE_H();	//�ø�CE
}
