 #ifndef __5110LCD_H
 #define __5110LCD_H

 #include "stm32f10x.h"	
///////////////////////////////////////////////////////
//------------------------------------/				 //
// SCE    ------------PA0  //片选					 //
// RST    ------------PA1  //位选					 //
// D/C    ------------PA2  // 1写数据，0写指令		 //
// SD     ------------PA3	 //数据				 //
// SCLK   ------------PA8	 //时钟				 //
///////////////////////////////////////////////////////
 #define sce0   GPIO_ResetBits(GPIOA, GPIO_Pin_0)  //片选
 #define res0   GPIO_ResetBits(GPIOA, GPIO_Pin_1)  //复位,0复位
 #define dc0    GPIO_ResetBits(GPIOA, GPIO_Pin_2)   //1写数据，0写指令
 #define sdin0  GPIO_ResetBits(GPIOA, GPIO_Pin_3)   //数据
 #define sclk0  GPIO_ResetBits(GPIOA, GPIO_Pin_8)   //时钟


 #define sce1   GPIO_SetBits(GPIOA, GPIO_Pin_0)  //片选
 #define res1   GPIO_SetBits(GPIOA, GPIO_Pin_1)  //复位,0复位
 #define dc1    GPIO_SetBits(GPIOA, GPIO_Pin_2)   //1写数据，0写指令
 #define sdin1  GPIO_SetBits(GPIOA, GPIO_Pin_3)   //数据
 #define sclk1  GPIO_SetBits(GPIOA, GPIO_Pin_8)   //时钟
	    
void GPIO_Configuration_LCD(void);
void LCD_WriteByte(unsigned char dt, unsigned char command);
void LCD_Init(void);
void LCD_Set_XY(unsigned char X, unsigned char Y);
void LCD_Clear(void);
void LCD_WriteChar(unsigned char X,unsigned char Y,unsigned char c);
void LCD_WriteString_en(unsigned char X,unsigned char Y,unsigned char *s);
void LCD_WriteNumber(unsigned char x,unsigned char y, unsigned long number);
void LCD_write_chinese_string(unsigned char X, unsigned char Y,
                              unsigned char ch_with,unsigned char num,
							  unsigned char line,unsigned char row);
void LCD_Write_cn(unsigned char row, unsigned char page,unsigned char c); //row:列 page:页 dd:字符

void delay_ms(int t);
#endif
