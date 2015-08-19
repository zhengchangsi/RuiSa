#include "oled.h"
#include "code.h"
#include "stdio.h"
/*********************OLEDÇý¶¯³ÌÐòÓÃµÄÑÓÊ±³ÌÐò***********************************
void delay(u16 z)
{
	u16 x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}*/

/**********************************************
//IIC Start
**********************************************/
void IIC_Start_OLED(void)
{
   SCL = high;		
   SDA = high;
   SDA = low;
   SCL = low;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop_OLED(void)
{
   SCL = low;
   SDA = low;
   SCL = high;
   SDA = high;
}

/**********************************************
// Í¨¹ýI2C×ÜÏßÐ´Ò»¸ö×Ö½Ú
**********************************************/
void Write_IIC_Byte(u8 IIC_Byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(IIC_Byte & 0x80)
			SDA=high;
		else
			SDA=low;
		SCL=high;
		SCL=low;
		IIC_Byte<<=1;
	}
	SDA=1;
	SCL=1;
	SCL=0;
}

/*********************OLEDÐ´Êý¾Ý************************************/ 
void OLED_WrDat(u8 IIC_Data)
{
	IIC_Start_OLED();
	Write_IIC_Byte(0x78);
	Write_IIC_Byte(0x40);			//write data
	Write_IIC_Byte(IIC_Data);
	IIC_Stop_OLED();
}
/*********************OLEDÐ´ÃüÁî************************************/
void OLED_WrCmd(u8 IIC_Command)
{
	IIC_Start_OLED();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	Write_IIC_Byte(0x00);			//write command
	Write_IIC_Byte(IIC_Command);
	IIC_Stop_OLED();
}
/*********************OLED ÉèÖÃ×ø±ê************************************/
void OLED_Set_Pos(u8 x, u8 y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLEDÈ«ÆÁ************************************/
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
/*********************OLED¸´Î»************************************/
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
/*********************OLED³õÊ¼»¯************************************/
void OLED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOC, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOE, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOE, GPIO_Pin_6);  
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
	
	delay_ms(500);//³õÊ¼»¯Ö®Ç°µÄÑÓÊ±ºÜÖØÒª£¡
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0×óÓÒ·´ÖÃ 0xa1Õý³£
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0ÉÏÏÂ·´ÖÃ 0xc8Õý³£
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
	OLED_Fill(0x00); //³õÊ¼ÇåÆÁ
	OLED_Set_Pos(0,0);
} 
/***************¹¦ÄÜÃèÊö£ºÏÔÊ¾6*8Ò»×é±ê×¼ASCII×Ö·û´®	ÏÔÊ¾µÄ×ø±ê£¨x,y£©£¬yÎªÒ³·¶Î§0¡«7****************/
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
/*******************¹¦ÄÜÃèÊö£ºÏÔÊ¾8*16Ò»×é±ê×¼ASCII×Ö·û´®	 ÏÔÊ¾µÄ×ø±ê£¨x,y£©£¬yÎªÒ³·¶Î§0¡«7****************/
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
/*****************¹¦ÄÜÃèÊö£ºÏÔÊ¾Êý×Ö£¬×î¶5Î»£¬ÏÔÊ¾µÄ×ø±ê£¨x,y£©£¬yÎªÒ³·¶Î§0¡«7****************************/
void OLED_Shownum(u8 x, u8 y, u8 size, u16 num)
{
	char str[5];
	
	sprintf(str, "%d", num);
	if (size == 0)
		OLED_P6x8Str(x, y, str);
	else if (size == 1)
		OLED_P8x16Str(x, y, str);
}
void OLED_Shownumf(u8 x, u8 y, u8 size, double num)
{
	char str[5];
	
	sprintf(str, "%6.2f", num);
	if (size == 0)
		OLED_P6x8Str(x, y, str);
	else if (size == 1)
		OLED_P8x16Str(x, y, str);
}
/*****************¹¦ÄÜÃèÊö£ºÏÔÊ¾16*16µãÕó  ÏÔÊ¾µÄ×ø±ê£¨x,y£©£¬yÎªÒ³·¶Î§0¡«7****************************/
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
/***********¹¦ÄÜÃèÊö£ºÏÔÊ¾´Óaµ½bµÄ16*16µãÕó  ÏÔÊ¾µÄ×ø±ê£¨x,y£©£¬yÎªÒ³·¶Î§0¡«7*****************/
void OLED_Chinese(u8 x, u8 y, u8 a, u8 b)
{
	u8 i;
	
	for (i=a; i<=b;i++)
	{
		OLED_P16x16Ch(x+(i-a)*16, y, i);
	}
}
/***********¹¦ÄÜÃèÊö£ºÏÔÊ¾ÏÔÊ¾BMPÍ¼Æ¬128¡Á64ÆðÊ¼µã×ø±ê(x,y),xµÄ·¶Î§0¡«127£¬yÎªÒ³µÄ·¶Î§0¡«7*****************/
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
