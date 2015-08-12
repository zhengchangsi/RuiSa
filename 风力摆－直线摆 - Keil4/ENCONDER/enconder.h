#ifndef __enconder_h__
#define __enconder_h__

#include "sys.h"

void Enconder_init(void);
s16 ENC1_Get_Electrical_Angle(void);
s16 ENC2_Get_Electrical_Angle(void);
#define D_Location_ERROR 200
#define MAX_COUNT 10000
#define ENCODER_TIM_PERIOD 0xffff


#endif

