#include <stm32f10x.h>
#include "SPI.h"
#include "NRF24L01.h"

const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = { 0xE1, 0xE2, 0xE3, 0xE4, 0xE5 };	//本地地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = { 0xE1, 0xE2, 0xE3, 0xE4, 0xE5 };	//接收地址

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

	NRF_Write_Buf(W_REGISTER + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);	//写RX节点地址
	NRF_Write_Buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);		//写TX节点地址
	NRF_Write_Reg(W_REGISTER + EN_AA, 0x01);													//使能通道0的自动应答
	NRF_Write_Reg(W_REGISTER + EN_RXADDR, 0x01);											//使能通道0的接收地址 
	NRF_Write_Reg(W_REGISTER + SETUP_RETR, 0x1a);											//设置自动重发间隔时间:500us;最大自动重发次数:10次 
	NRF_Write_Reg(W_REGISTER + RF_CH, channel);												//设置RF通道为CHANAL
	NRF_Write_Reg(W_REGISTER + RF_SETUP, 0x0f);												//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	//NRF_Write_Reg(W_REGISTER + RF_SETUP,0x07);											//设置TX发射参数,0db增益,1Mbps,低噪声增益开启

	if (mode == MODE_RX)				//RX
	{
		NRF_Write_Reg(W_REGISTER + RX_PW_P0, RX_PLOAD_WIDTH);	//选择通道0的有效数据宽度 
		NRF_Write_Reg(W_REGISTER + CONFIG, 0x0f);					//IRQ收发完成中断开启,16位CRC,主接收
	}
	else if (mode == MODE_TX)		//TX
	{
		NRF_Write_Reg(W_REGISTER + RX_PW_P0, RX_PLOAD_WIDTH);	//选择通道0的有效数据宽度 
		NRF_Write_Reg(W_REGISTER + CONFIG, 0x0e);					//IRQ收发完成中断开启,16位CRC,主发送
	}
	else if (mode == MODE_RX2)	//RX2
	{
		NRF_Write_Reg(FLUSH_TX, 0xff);
		NRF_Write_Reg(FLUSH_RX, 0xff);
		NRF_Write_Reg(W_REGISTER + CONFIG, 0x0f);					//IRQ收发完成中断开启,16位CRC,主接收

		SPI_ReadWrite(0x50);
		SPI_ReadWrite(0x73);
		NRF_Write_Reg(W_REGISTER + DYNPD, 0x01);
		NRF_Write_Reg(W_REGISTER + FEATURE, 0x06);
	}
	else												//TX2
	{
		NRF_Write_Reg(W_REGISTER + CONFIG, 0x0e);					//IRQ收发完成中断开启,16位CRC,主发送
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

	NRF_Write_Buf(W_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	//写入5个字节的地址
	NRF_Read_Buf(TX_ADDR, buffer, TX_ADR_WIDTH);	//读出写入的地址

	for (i = 0; i < 5; i++)	//比较
	{
		if (buffer[i] != TX_ADDRESS[i])
			break;
	}
	if (i >= 5)
		return 1;	//MCU与NRF成功连接 
	else
		return 0;		//MCU与NRF不正常连接 
}

/*
*****************************************************************
* 写寄存器
*****************************************************************
*/
uint8_t NRF_Write_Reg(uint8_t WriteAddr, uint8_t uData)
{
	uint8_t status;

	SPI_NSS_L();												/* 选通器件 */
	status = SPI_ReadWrite(WriteAddr);	/* 写寄存器写地址 */
	SPI_ReadWrite(uData);		  				/* 写数据 */
	SPI_NSS_H();												/* 禁止该器件 */
	return status;
}
/*
*****************************************************************
* 读寄存器
*****************************************************************
*/
uint8_t NRF_Read_Reg(uint8_t ReadAddr)
{
	uint8_t uData;

	SPI_NSS_L();								/* 选通器件 */
	SPI_ReadWrite(ReadAddr);		/* 写寄存器读地址 */
	uData = SPI_ReadWrite(0);	/* 读取该寄存器返回数据 */
	SPI_NSS_H();								/* 禁止该器件 */
	return uData;
}
/*
*****************************************************************
* 写缓冲区
*****************************************************************
*/
uint8_t NRF_Write_Buf(uint8_t WriteAddr, const uint8_t* pBuffer, uint8_t NumByteToWrite)
{
	uint8_t status;

	SPI_NSS_L();												/* 选通器件 */
	status = SPI_ReadWrite(WriteAddr);	/* 写寄存器地址 */
	while (NumByteToWrite--)
	{
		SPI_ReadWrite(*pBuffer);					/* 写数据 */
		pBuffer++;
	}
	SPI_NSS_H();												/* 禁止该器件 */
	return status;
}
/*
*****************************************************************
* 读缓冲区
*****************************************************************
*/
uint8_t NRF_Read_Buf(uint8_t ReadAddr, uint8_t* pBuffer, uint8_t NumByteToRead)
{
	uint8_t status;

	SPI_NSS_L();												/* 选通器件 */
	status = SPI_ReadWrite(ReadAddr);	/* 写寄存器地址 */
	while (NumByteToRead--)
	{
		*pBuffer = SPI_ReadWrite(0);			/* 读取返回数据 */
		pBuffer++;
	}
	SPI_NSS_H();												/* 禁止该器件 */
	return status;
}
/*
*****************************************************************
* 写数据包
*****************************************************************
*/
void NRF_TxPacket(const uint8_t* pBuffer, uint8_t NumByteToWrite)
{
	NRF24L01_CE_L();	//StandBy I模式	
	NRF_Write_Buf(W_REGISTER + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);	//装载接收端地址，用于接收应答
	NRF_Write_Buf(W_TX_PLOAD, pBuffer, NumByteToWrite);	//装载数据	
	NRF24L01_CE_H();	//置高CE，激发数据发送
}

void NRF_TxPacket_AP(const uint8_t* pBuffer, uint8_t NumByteToWrite)
{
	NRF24L01_CE_L();	//StandBy I模式
	NRF_Write_Buf(W_ACK_PAYLOAD, pBuffer, NumByteToWrite);	//装载数据
	NRF24L01_CE_H();	//置高CE
}
