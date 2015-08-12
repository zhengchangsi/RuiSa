#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <stm32f10x.h>

typedef int16_t Message;

#define TxBufferSize 16

void SendMessage(Message parameter);

#endif // __MESSAGE_H
