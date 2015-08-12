#ifndef __OLED_H__
#define __OLED_H__
#include "sys.h"
#include "delay.h"
#define SCL PAout(4) //����ʱ��
#define SDA PAout(5) //��������

#define high 1
#define low 0

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
//��������
//void delay(u16 z);// ��ʱ����,����
void IIC_Start(void);// ����I2C����
void IIC_Stop(void);// �ر�I2C����
void Write_IIC_Byte(u8 IIC_Byte);// ͨ��I2C����дһ��byte������
void OLED_WrDat(u8 dat);// ��OLED��д����
void OLED_WrCmd(u8 cmd);// ��OLED��д����
void OLED_Set_Pos(u8 x, u8 y);// ������ʾ����
void OLED_Fill(u8 bmp_dat);// ȫ����ʾ(��ʾBMPͼƬʱ�Ż��õ��˹���)
void OLED_CLS(void);// ��λ/����
void OLED_Init(void);// OLED����ʼ�����򣬴˺���Ӧ�ڲ�����Ļ֮ǰ���ȵ���
void OLED_P6x8Str(u8 x, u8 y,u8 ch[]);// 6x8������������ʾASCII�����С���У���̫����
void OLED_P8x16Str(u8 x, u8 y,u8 ch[]);// 8x16������������ʾASCII�룬�ǳ����
void OLED_Shownum(u8 x, u8 y, u8 size, u16 num);
void OLED_Shownumf(u8 x, u8 y, u8 size, double num);
void OLED_P16x16Ch(u8 x, u8 y, u8 N);// 16x16������������ʾ���ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
void OLED_Chinese(u8 x, u8 y, u8 a, u8 b);// 16x16������������ʾ��a��b�ĺ��ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
void Draw_BMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);// ��128x64���ص�BMPλͼ��ȡ�����������ֱ�Ȼ���Ƶ�codetab�У��˺������ü���



void Show_PID(float P1, float I1, float D1,float P2, float I2, float D2);
#endif
