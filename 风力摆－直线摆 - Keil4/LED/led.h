#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED PBout(9)// PB0

void LED_Init(void);//≥ı ºªØ

void LED_OFF(void);
void LED_ON(void);

void Beep_ON(void);
void Beep_OFF(void);
void Beep_warning(void);

		 				    
#endif
