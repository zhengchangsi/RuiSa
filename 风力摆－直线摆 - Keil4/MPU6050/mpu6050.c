#include "stm32f10x.h"
#include "sys.h"
#include "mpu6050.h"
#include <stdio.h>   //Keil library	
#include "delay.h"
#include "oled.h"


#define MPU6050_FILTER_NUM 10
#define MSS5611_FILTER_NUM 20


S_INT16_XYZ	 MPU6050_ACC_LAST, MPU6050_GYRO_LAST;//����һ�ζ�ȡֵ,��Ϊhmc5883l�����ݿ���ͨ��MPU6050��ȡ��
S_INT16_XYZ	 GYRO_OFFSET, ACC_OFFSET;//������ٶ�����ٶ���ƫ
S_INT16_XYZ	 ACC_AVG;


//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(2);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(2);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	delay_us(1);
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(2);
	IIC_SCL=1;
	delay_us(1);	
	IIC_SDA=1;//����I2C���߽����ź�
	//delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;
	delay_us(1);	   
	IIC_SCL=1;
	delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SDA_OUT();
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
//        IIC_SDA=(txd&0x80)>>7;
//        txd<<=1; 
				
				if (txd&0x80) IIC_SDA=1;
				else 	IIC_SDA=0;
				txd <<=1;
				delay_us(2);   //��TEA5767��������ʱ���Ǳ����
				IIC_SCL=1;
				delay_us(2); 
				IIC_SCL=0;	
				delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
		receive<<=1;
    IIC_SCL=0; 
    delay_us(2);
		IIC_SCL=1;
		delay_us(2);
    if(READ_SDA)receive|=0x01;   
		delay_us(2); 
  }	
		IIC_SCL=0; 
		SDA_OUT();
    if (!ack)IIC_NAck();//����nACK
    else IIC_Ack(); //����ACK   
    return receive;
}
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data)
{
	IIC_Start();
	IIC_Send_Byte(daddr);//������ַ+д�ź�
	IIC_Wait_Ack();
	IIC_Send_Byte(addr);//�Ĵ���
	IIC_Wait_Ack();
	IIC_Send_Byte(data);//����
	IIC_Wait_Ack();
	IIC_Stop();
}
u8 IIC_Read_One_Byte(u8 daddr,u8 addr)
{
	u8 temp;
	IIC_Start();
	IIC_Send_Byte(daddr);//������ַ+д�ź�
	IIC_Wait_Ack();
	
	IIC_Send_Byte(addr);//�Ĵ���
	IIC_Wait_Ack();
	
	IIC_Start(); 
	IIC_Send_Byte(daddr+1);//������ַ+���ź�
	IIC_Wait_Ack();
	
	temp = IIC_Read_Byte(0);
	IIC_Stop();
	return temp;
}

//**************************************
//��ʼ��MPU6050
//**************************************
void MPU6050_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);    
	
	delay_ms(1000);//��ʼ��֮ǰ����ʱ����Ҫ��
	
	IIC_Write_One_Byte(MPU6050_SlaveAddress,PWR_MGMT_1, 0x00);	//�������״̬
	delay_ms(1000);
	IIC_Write_One_Byte(MPU6050_SlaveAddress,SMPLRT_DIV, 0x00);
	delay_ms(1000);
	IIC_Write_One_Byte(MPU6050_SlaveAddress,CONFIG, 0x06);
	delay_ms(1000);
	IIC_Write_One_Byte(MPU6050_SlaveAddress,GYRO_CONFIG, 0x18);
	delay_ms(1000);
	IIC_Write_One_Byte(MPU6050_SlaveAddress,ACCEL_CONFIG, 0x08);
}
uint16_t GetData_MPU6050(uint8_t REG_Address)
{
	uint8_t H,L;
	
	H=IIC_Read_One_Byte(MPU6050_SlaveAddress,REG_Address);
	L=IIC_Read_One_Byte(MPU6050_SlaveAddress,REG_Address+1);
	return (H<<8)|L;   //�ϳ�����

}

/*************************************
 * ��������Read_Filter_MPU6050
 * ����  ����MPU6050�ļ��ٶ�����ٶȵ�ֵ��
					 ����ǵ�һ�ε��øú�������������
					 �������ƫ������ʱ�򲻼��㣻
					 �����˲���
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 ************************************/
uint8_t Read_Filter_MPU6050(void)
{
	//static uint8_t	GYRO_OFFSET_OK = 0;
   static uint8_t	ACC_OFFSET_OK  = 0;
	static uint8_t filter_cnt= 0;
  static int16_t	ACC_X_BUF[MPU6050_FILTER_NUM],ACC_Y_BUF[MPU6050_FILTER_NUM],ACC_Z_BUF[MPU6050_FILTER_NUM];		
	int32_t temp1=0,temp2=0,temp3=0;
	uint8_t i;
	
	ACC_OFFSET.X=0;
	ACC_OFFSET.Y=0;
	ACC_OFFSET.Z=0;
	#if 1
	GYRO_OFFSET.X=-45;
	GYRO_OFFSET.Y=0;
	GYRO_OFFSET.Z=15;
	#endif

	MPU6050_ACC_LAST.X=GetData_MPU6050(ACCEL_XOUT_H)- ACC_OFFSET.X;
	MPU6050_ACC_LAST.Y=GetData_MPU6050(ACCEL_YOUT_H)- ACC_OFFSET.Y;
	MPU6050_ACC_LAST.Z=GetData_MPU6050(ACCEL_ZOUT_H)- ACC_OFFSET.Z;
	
	MPU6050_GYRO_LAST.X=GetData_MPU6050(GYRO_XOUT_H)- GYRO_OFFSET.X;
	MPU6050_GYRO_LAST.Y=GetData_MPU6050(GYRO_YOUT_H)- GYRO_OFFSET.Y;
	MPU6050_GYRO_LAST.Z=GetData_MPU6050(GYRO_ZOUT_H)- GYRO_OFFSET.Z;
//	if(!GYRO_OFFSET_OK)
//	{
//		static int32_t	tempgx=0,tempgy=0,tempgz=0;
//		static uint8_t cnt_g=0;
//		if(cnt_g==0)
//		{
//			GYRO_OFFSET.X=0;
//			GYRO_OFFSET.Y=0;
//			GYRO_OFFSET.Z=0;
//			tempgx = 0;
//			tempgy = 0;
//			tempgz = 0;
//			cnt_g = 1;
//			return 0;
//		}
//		tempgx+= MPU6050_GYRO_LAST.X;
//		tempgy+= MPU6050_GYRO_LAST.Y;
//		tempgz+= MPU6050_GYRO_LAST.Z;
//		if(cnt_g==200)
//		{
//			GYRO_OFFSET.X=tempgx/cnt_g;
//			GYRO_OFFSET.Y=tempgy/cnt_g;
//			GYRO_OFFSET.Z=tempgz/cnt_g;
//			cnt_g = 0;
//			GYRO_OFFSET_OK = 1;
//			return 1;
//		}
//		cnt_g++;
//		}
	
	if(!ACC_OFFSET_OK)
	{
		static int32_t	tempax=0,tempay=0,tempaz=0;
		static uint8_t cnt_a=0;
		if(cnt_a==0)
		{
			ACC_OFFSET.X = 0;
			ACC_OFFSET.Y = 0;
			ACC_OFFSET.Z = 0;
			tempax = 0;
			tempay = 0;
			tempaz = 0;
			cnt_a = 1;
			return 0;
		}
		tempax+= MPU6050_ACC_LAST.X;
		tempay+= MPU6050_ACC_LAST.Y;
		tempaz+= MPU6050_ACC_LAST.Z;
		if(cnt_a==200)
		{
			ACC_OFFSET.X=tempax/cnt_a;
			ACC_OFFSET.Y=tempay/cnt_a;
			ACC_OFFSET.Z=tempaz/cnt_a;
			cnt_a = 0;
			ACC_OFFSET_OK = 1;
			return 1;
		}
		cnt_a++;		
	}
	
  ACC_X_BUF[filter_cnt] = MPU6050_ACC_LAST.X;
	ACC_Y_BUF[filter_cnt] = MPU6050_ACC_LAST.Y;
	ACC_Z_BUF[filter_cnt] = MPU6050_ACC_LAST.Z;
	for(i=0;i<MPU6050_FILTER_NUM;i++)
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
	}
	ACC_AVG.X = temp1 / MPU6050_FILTER_NUM;
	ACC_AVG.Y = temp2 / MPU6050_FILTER_NUM;
	ACC_AVG.Z = temp3 / MPU6050_FILTER_NUM;
	filter_cnt++;
	if(filter_cnt==MPU6050_FILTER_NUM)	filter_cnt=0;
	return 1;
}
