#ifndef __OLED_H__
#define __OLED_H__
#include "sys.h"
#include "delay.h"
#define SCL PAout(4) //´®ĞĞÊ±ÖÓ
#define SDA PAout(5) //´®ĞĞÊı¾İ

#define high 1
#define low 0

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
//º¯ÊıÉùÃ÷
//void delay(u16 z);// ÑÓÊ±º¯Êı,ºÁÃë
void IIC_Start(void);// ¿ªÆôI2C×ÜÏß
void IIC_Stop(void);// ¹Ø±ÕI2C×ÜÏß
void Write_IIC_Byte(u8 IIC_Byte);// Í¨¹ıI2C×ÜÏßĞ´Ò»¸öbyteµÄÊı¾İ
void OLED_WrDat(u8 dat);// ÏòOLEDÆÁĞ´Êı¾İ
void OLED_WrCmd(u8 cmd);// ÏòOLEDÆÁĞ´ÃüÁî
void OLED_Set_Pos(u8 x, u8 y);// ÉèÖÃÏÔÊ¾×ø±ê
void OLED_Fill(u8 bmp_dat);// È«ÆÁÏÔÊ¾(ÏÔÊ¾BMPÍ¼Æ¬Ê±²Å»áÓÃµ½´Ë¹¦ÄÜ)
void OLED_CLS(void);// ¸´Î»/ÇåÆÁ
void OLED_Init(void);// OLEDÆÁ³õÊ¼»¯³ÌĞò£¬´Ëº¯ÊıÓ¦ÔÚ²Ù×÷ÆÁÄ»Ö®Ç°×îÏÈµ÷ÓÃ
void OLED_P6x8Str(u8 x, u8 y,u8 ch[]);// 6x8µãÕû£¬ÓÃÓÚÏÔÊ¾ASCIIÂëµÄ×îĞ¡ÕóÁĞ£¬²»Ì«ÇåÎú
void OLED_P8x16Str(u8 x, u8 y,u8 ch[]);// 8x16µãÕû£¬ÓÃÓÚÏÔÊ¾ASCIIÂë£¬·Ç³£ÇåÎ
void OLED_Shownum(u8 x, u8 y, u8 size, u16 num);
void OLED_Shownumf(u8 x, u8 y, u8 size, double num);
void OLED_P16x16Ch(u8 x, u8 y, u8 N);// 16x16µãÕû£¬ÓÃÓÚÏÔÊ¾ºº×ÖµÄ×îĞ¡ÕóÁĞ£¬¿ÉÉèÖÃ¸÷ÖÖ×ÖÌå¡¢¼Ó´Ö¡¢ÇãĞ±¡¢ÏÂ»®ÏßµÈ
void OLED_Chinese(u8 x, u8 y, u8 a, u8 b);// 16x16µãÕû£¬ÓÃÓÚÏÔÊ¾´Óaµ½bµÄºº×ÖµÄ×îĞ¡ÕóÁĞ£¬¿ÉÉèÖÃ¸÷ÖÖ×ÖÌå¡¢¼Ó´Ö¡¢ÇãĞ±¡¢ÏÂ»®ÏßµÈ
void Draw_BMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);// ½«128x64ÏñËØµÄBMPÎ»Í¼ÔÚÈ¡×ÖÈí¼şÖĞËã³ö×Ö±í£¬È»ºó¸´ÖÆµ½codetabÖĞ£¬´Ëº¯Êıµ÷ÓÃ¼´¿É



void Show_PID(float P1, float I1, float D1,float P2, float I2, float D2);
#endif
