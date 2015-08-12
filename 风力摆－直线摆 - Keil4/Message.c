#include <stm32f10x.h>
#include "NRF24L01.h"
#include "Message.h"

static Message TxBuffer[TxBufferSize];	//·¢ËÍ»º³åÇø
static uint32_t TxBufferIndex = 0;	//×¢Òâ·¶Î§

void SendMessage(Message parameter)
{
	TxBuffer[TxBufferIndex] = parameter;
	TxBufferIndex++;

	if (TxBufferIndex == TxBufferSize)
	{
		TxBufferIndex = 0;

		NRF_TxPacket((uint8_t *)TxBuffer, sizeof(TxBuffer));
	}
}
