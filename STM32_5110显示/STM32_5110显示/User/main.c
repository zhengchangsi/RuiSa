/**********************************************************************
* œ÷œÛ£∫5110LCDœ‘ æ°∞Hangzhoudianzi ª∂”≠ƒ„°±

***********************************************************************/
#include "stm32f10x.h"
#include "5110LCD.h"
#include "key.h"


extern unsigned char chinese1;
void Delays(__IO u32 nCount);

int main(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		Key_GPIO_Config();
		GPIO_Configuration_LCD();
		Init_LED();
		LCD_Init();
		LCD_Clear();

		while (1)
		{
			LCD_WriteString_en(0,0,"   5402 lib");
			if(Key_Scan(GPIOE,GPIO_Pin_0) == KEY_ON)
			{
				LED1_TOGGLE;
			}
			
			LCD_Write_cn(20, 2, 0);
			LCD_Write_cn(40, 2, 1);
			LCD_Write_cn(60, 2, 2);
		}					
}

void Delays(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 

