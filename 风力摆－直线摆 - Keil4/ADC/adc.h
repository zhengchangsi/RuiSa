#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//#include "config.h"

extern volatile unsigned short ADC_ConvertedValue[3];

void Adc_Init(void);
//u16  Get_Adc(u8 ch); 
//void Get_Adc_Average(void);
//void Voltage_Deal(void);

 
#endif 


