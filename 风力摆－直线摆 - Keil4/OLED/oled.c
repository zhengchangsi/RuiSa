#include "oled.h"
#include "code.h"
#include "stdio.h"

#include "mpu6050.h"


/*********************OLED���������õ���ʱ����***********************************
void delay(u16 z)
{
	u16 x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}*/

///**********************************************
////IIC Start
//**********************************************/
//void IIC_Start(void)
//{
//   SCL = high;		
//   SDA = high;
//   SDA = low;
//   SCL = low;
//}

///**********************************************
////IIC Stop
//**********************************************/
//void IIC_Stop(void)
//{
//   SCL = low;
//   SDA = low;
//   SCL = high;
//   SDA = high;
//}

///**********************************************
//// ͨ��I2C����дһ���ֽ�
//**********************************************/
//void Write_IIC_Byte(u8 IIC_Byte)
//{
//	u8 i;
//	for(i=0;i<8;i++)
//	{
//		if(IIC_Byte & 0x80)
//			SDA=high;
//		else
//			SDA=low;
//		SCL=high;
//		SCL=low;
//		IIC_Byte<<=1;
//	}
//	SDA=1;
//	SCL=1;
//	SCL=0;
//}

/*********************OLEDд����************************************/ 
void OLED_WrDat(u8 IIC_Data)
{
	IIC_Write_One_Byte(0x78,0x40,IIC_Data);
}
/*********************OLEDд����************************************/
void OLED_WrCmd(u8 IIC_Command)
{
	IIC_Write_One_Byte(0x78,0x00,IIC_Command);
}
/*********************OLED ��������************************************/
void OLED_Set_Pos(u8 x, u8 y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLEDȫ��************************************/
void OLED_Fill(u8 bmp_dat) 
{
	u8 y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}
/*********************OLED��λ************************************/
void OLED_CLS(void)
{
	u8 y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(0);
	}
}
/*********************OLED��ʼ��************************************/
void OLED_Init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
//	GPIO_Init(GPIOA, &GPIO_InitStructure);					
//	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);    
//	delay_ms(500);//��ʼ��֮ǰ����ʱ����Ҫ��
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //��ʼ����
	OLED_Set_Pos(0,0);
} 
/***************������������ʾ6*8һ���׼ASCII�ַ���	��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P6x8Str(u8 x, u8 y,u8 ch[])
{
	u8 c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}
/*******************������������ʾ8*16һ���׼ASCII�ַ���	 ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P8x16Str(u8 x, u8 y,u8 ch[])
{
	u8 c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
/*****************������������ʾ���֣���5λ����ʾ�����꣨x,y����yΪҳ��Χ0��7****************************/
void OLED_Shownum(u8 x, u8 y, u8 size, u16 num)
{
	char str[5];
	
	sprintf(str, "%d", num);
	if (size == 0)
		OLED_P6x8Str(x, y, (u8*)str);
	else if (size == 1)
		OLED_P8x16Str(x, y, (u8*)str);
}
void OLED_Shownumf(u8 x, u8 y, u8 size, double num)
{
	char str[5];
	
	sprintf(str, "%.2f", num);
	if (size == 0)
		OLED_P6x8Str(x, y, (u8*)str);
	else if (size == 1)
		OLED_P8x16Str(x, y, (u8*)str);
}
/*****************������������ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************************/
void OLED_P16x16Ch(u8 x, u8 y, u8 N)
{
	u8 wm=0;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[2*N][wm]);
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[2*N+1][wm]);
	} 	  	
}
/***********������������ʾ��a��b��16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7*****************/
void OLED_Chinese(u8 x, u8 y, u8 a, u8 b)
{
	u8 i;
	
	for (i=a; i<=b;i++)
	{
		OLED_P16x16Ch(x+(i-a)*16, y, i);
	}
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void Draw_BMP(u8 x0, u8 y0, u8 x1, u8 y1,u8 BMP[])
{
	u16 j=0;
	u8 x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);
	    }
	}
}



//void Show_PID(float P1, float I1, float D1,float P2, float I2, float D2)
//{
//	uint8_t str_temp[15] = 0;
//	
//	sprintf(str_temp, "P1:%7.2f", P1);	
//	OLED_P6x8Str(1, 2, str_temp);
//	sprintf(str_temp, "I1:%7.2f", I1);
//	OLED_P6x8Str(1, 3, str_temp);
//	sprintf(str_temp, "D1:%7.2f", D1);
//	OLED_P6x8Str(1, 4, str_temp);

//	sprintf(str_temp, "P2:%7.2f", P2);
//	OLED_P6x8Str(1, 5, str_temp);
//	sprintf(str_temp, "I2:%7.2f", I2);
//	OLED_P6x8Str(1, 6, str_temp);
//	sprintf(str_temp, "D2:%7.2f", D2);
//	OLED_P6x8Str(1, 7, str_temp);


//}

