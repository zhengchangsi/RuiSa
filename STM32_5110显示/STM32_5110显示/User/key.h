#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"
 /*******
 *按键按下标置
 KEY_ON 0
 KEY_OFF 1
 ********/
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)				{p->BSRR=i;}			//设置为高电平		
#define digitalLo(p,i)				{p->BRR	=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(GPIOG,GPIO_Pin_14)
#define LED1_OFF			digitalHi(GPIOG,GPIO_Pin_14)
#define LED1_ON				digitalLo(GPIOG,GPIO_Pin_14)

/*#define LED2_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_7)
#define LED2_OFF			digitalHi(GPIOF,GPIO_Pin_7)
#define LED2_ON				digitalLo(GPIOF,GPIO_Pin_7)

#define LED3_TOGGLE		digitalToggle(GPIOF,GPIO_Pin_8)
#define LED3_OFF			digitalHi(GPIOF,GPIO_Pin_8)
#define LED3_ON				digitalLo(GPIOF,GPIO_Pin_8) */
 
#define KEY_ON	0
#define KEY_OFF	1

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);
void Init_LED(void);

#endif /* __LED_H */
