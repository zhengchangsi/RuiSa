#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "sys.h"

typedef struct{
				int16_t X;
				int16_t Y;
				int16_t Z;}S_INT16_XYZ;//���Ͷ���
typedef struct{
				float X;
				float Y;
				float Z;}T_FLOAT_XYZ;
typedef struct{
				float rol;
				float pit;
				float yaw;}T_FLOAT_ANGLE;				


extern S_INT16_XYZ	 MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//����һ�ζ�ȡֵ,��Ϊhmc5883l�����ݿ���ͨ��MPU6050��ȡ��
extern S_INT16_XYZ	 GYRO_OFFSET,ACC_OFFSET;//������ٶ�����ٶ���ƫ
extern S_INT16_XYZ	 ACC_AVG;

//MPU6050��������
void MPU6050_Init(void);													//��ʼ��MPU6050
void IIC_Init(void);              							  //Initialize the IIC IO port                       
void IIC_Start(void);                        		  //Send the IIC start signal
void IIC_Stop(void);                       			  //Send the IIC stop signal
void IIC_Send_Byte(u8 txd);               			  //IIC sends a byte
u8   IIC_Read_Byte(unsigned char ack); 							//IIC read a byte
u8   IIC_Wait_Ack(void);                     				//IIC wait for an ACK signal
void IIC_Ack(void);                           		//IIC an ACK signal
void IIC_NAck(void);                        			//IIC is not an ACK signal
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8   IIC_Read_One_Byte(u8 daddr,u8 addr);   

					
uint8_t Read_Filter_MPU6050(void);


#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<11;}
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<11;}

#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  //����SDA 



//****************************************
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG				0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
//#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
#define	MPU6050_SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ�����


#endif

