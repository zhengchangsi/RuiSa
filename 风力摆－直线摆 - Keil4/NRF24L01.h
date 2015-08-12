#ifndef __NRF24L01_H
#define __NRF24L01_H

#include <stm32f10x.h>

#define NRF24L01_CE_H()	GPIOB->BSRR = GPIO_Pin_1
#define NRF24L01_CE_L()	GPIOB->BRR = GPIO_Pin_1

//***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define R_REGISTER					0x00	//���Ĵ���ָ��
#define W_REGISTER					0x20	//д�Ĵ���ָ��
#define R_RX_PLOAD					0x61	//��ȡ��������ָ��
#define W_TX_PLOAD					0xA0	//д��������ָ��
#define FLUSH_TX						0xE1	//��ϴ���� FIFOָ��
#define FLUSH_RX						0xE2	//��ϴ���� FIFOָ��
#define REUSE_TX_PL					0xE3	//�����ظ�װ������ָ��
#define NOP									0xFF	//����

/* ��������nRF24L01+ */
#define R_RX_PL_WID					0x60
#define W_ACK_PAYLOAD				0xA8
#define W_TX_PAYLOAD_NOACK	0xB0

//*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG				0x00  //�����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA					0x01  //�Զ�Ӧ��������
#define EN_RXADDR			0x02  //�����ŵ�����
#define SETUP_AW			0x03  //�շ���ַ�������
#define SETUP_RETR		0x04  //�Զ��ط���������
#define RF_CH					0x05  //����Ƶ������
#define RF_SETUP			0x06  //�������ʡ����Ĺ�������
#define STATUS				0x07  //״̬�Ĵ���
#define OBSERVE_TX		0x08  //���ͼ�⹦��
#define RPD						0x09  //nRF24L01��ַ���(CD)��nRF24L01+���չ��ʼ��(RPD)
#define RX_ADDR_P0		0x0A  //Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1		0x0B  //Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2		0x0C  //Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3		0x0D  //Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4		0x0E  //Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5		0x0F  //Ƶ��5�������ݵ�ַ
#define TX_ADDR				0x10  //���͵�ַ�Ĵ���
#define RX_PW_P0			0x11  //����Ƶ��0�������ݳ���
#define RX_PW_P1			0x12  //����Ƶ��1�������ݳ���
#define RX_PW_P2			0x13  //����Ƶ��2�������ݳ���
#define RX_PW_P3			0x14  //����Ƶ��3�������ݳ���
#define RX_PW_P4			0x15  //����Ƶ��4�������ݳ���
#define RX_PW_P5			0x16  //����Ƶ��5�������ݳ���
#define FIFO_STATUS		0x17  //FIFOջ��ջ��״̬�Ĵ�������

/* ��������nRF24L01+ */
#define DYNPD					0x1C
#define FEATURE				0x1D

//*********************************************NRF24L01*************************************
#define RX_DR					6			//�жϱ�־
#define TX_DS					5
#define MAX_RT				4

#define MODE_RX				1			//��ͨ����
#define MODE_TX				2			//��ͨ����
#define MODE_RX2			3			//����ģʽ2,����˫����
#define MODE_TX2			4			//����ģʽ2,����˫����

#define RX_PLOAD_WIDTH  32
#define TX_PLOAD_WIDTH  32
#define TX_ADR_WIDTH    5
#define RX_ADR_WIDTH    5

void NRF24L01_Init(uint8_t mode, uint8_t channel);	//��ʼ��,mode=1/2/3/4
uint32_t NRF24L01_Check(void);

uint8_t NRF_Write_Reg(uint8_t WriteAddr, uint8_t uData);
uint8_t NRF_Read_Reg(uint8_t ReadAddr);
uint8_t NRF_Write_Buf(uint8_t WriteAddr, const uint8_t* pBuffer, uint8_t NumByteToWrite);
uint8_t NRF_Read_Buf(uint8_t ReadAddr, uint8_t* pBuffer, uint8_t NumByteToRead);

void NRF_TxPacket(const uint8_t* pBuffer, uint8_t NumByteToWrite);		//�������ݰ�,����mode 2/4
void NRF_TxPacket_AP(const uint8_t* pBuffer, uint8_t NumByteToWrite);	//�������ݰ�,����mode 3

#endif // __NRF24L01_H
