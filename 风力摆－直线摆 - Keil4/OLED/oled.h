#ifndef __OLED_H__
#define __OLED_H__
#include "sys.h"
#include "delay.h"
#define SCL PAout(4) //串行时钟
#define SDA PAout(5) //串行数据

#define high 1
#define low 0

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
//函数声明
//void delay(u16 z);// 延时函数,毫秒
void IIC_Start(void);// 开启I2C总线
void IIC_Stop(void);// 关闭I2C总线
void Write_IIC_Byte(u8 IIC_Byte);// 通过I2C总线写一个byte的数据
void OLED_WrDat(u8 dat);// 向OLED屏写数据
void OLED_WrCmd(u8 cmd);// 向OLED屏写命令
void OLED_Set_Pos(u8 x, u8 y);// 设置显示坐标
void OLED_Fill(u8 bmp_dat);// 全屏显示(显示BMP图片时才会用到此功能)
void OLED_CLS(void);// 复位/清屏
void OLED_Init(void);// OLED屏初始化程序，此函数应在操作屏幕之前最先调用
void OLED_P6x8Str(u8 x, u8 y,u8 ch[]);// 6x8点整，用于显示ASCII码的最小阵列，不太清晰
void OLED_P8x16Str(u8 x, u8 y,u8 ch[]);// 8x16点整，用于显示ASCII码，非常清�
void OLED_Shownum(u8 x, u8 y, u8 size, u16 num);
void OLED_Shownumf(u8 x, u8 y, u8 size, double num);
void OLED_P16x16Ch(u8 x, u8 y, u8 N);// 16x16点整，用于显示汉字的最小阵列，可设置各种字体、加粗、倾斜、下划线等
void OLED_Chinese(u8 x, u8 y, u8 a, u8 b);// 16x16点整，用于显示从a到b的汉字的最小阵列，可设置各种字体、加粗、倾斜、下划线等
void Draw_BMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);// 将128x64像素的BMP位图在取字软件中算出字表，然后复制到codetab中，此函数调用即可



void Show_PID(float P1, float I1, float D1,float P2, float I2, float D2);
#endif
