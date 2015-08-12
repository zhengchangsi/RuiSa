#include "stm32f10x.h"
#include "adc.h"
#include "oled.h"
#include "delay.h"
//#include "usart1.h"
//#include "control.h"
//#include "config.h"

#define ADC1_DR_Address  ((u32)0x40012400+0x4c)

volatile unsigned short ADC_ConvertedValue[3];

void RCC_Configuration_ADC(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void GPIO_Configuration_ADC(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* ��������ADC1_Mode_Config   
 * ����  ������ ADC1 �Ĺ���ģʽΪ dma ģʽ   
 * ����  : ��   
 * ���  ����   
 * ����  ���ڲ�����   
 */   
void ADC1_Configuration_DMA(void)   // unsigned char channel
{    
     ADC_InitTypeDef ADC_InitStructure;
     /* ADC1 configuration */     
     ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  /*���� ADC ģʽ*/    
     ADC_InitStructure.ADC_ScanConvMode = ENABLE ;   /*ʹ��ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�*/    
     ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  /*��������ת��ģʽ������ͣ�ؽ��� ADC ת��*/    
     ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; /*��ʹ���ⲿ����ת��*/    
     ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  /*�ɼ������Ҷ���*/    
     ADC_InitStructure.ADC_NbrOfChannel = 3;     /*Ҫת����ͨ����Ŀ 1*/    
     ADC_Init(ADC1, &ADC_InitStructure);    
       
     /*���� ADC ʱ�ӣ�Ϊ PCLK2 �� 8 ��Ƶ���� 9MHz*/ 
     RCC_ADCCLKConfig(RCC_PCLK2_Div8);  

//    if (channel == LEFT_CHANNEL) /*���� ADC1 ��ͨ�� 4 Ϊ 55.5 ���������� */     
     //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);
//    else if (channel == RIGHT_CHANNEL) /*���� ADC1 ��ͨ�� 5 Ϊ 55.5 ���������� */ 
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_28Cycles5); 
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_28Cycles5); 

	   
     /* Enable ADC1 DMA */   
     ADC_DMACmd(ADC1, ENABLE);   
     /* Enable ADC1 */   
     ADC_Cmd(ADC1, ENABLE);    
     /*��λУ׼�Ĵ��� */       
     ADC_ResetCalibration(ADC1);   
     /*�ȴ�У׼�Ĵ�����λ��� */   
     while(ADC_GetResetCalibrationStatus(ADC1));    
     /* ADC У׼ */   
     ADC_StartCalibration(ADC1);    
     /* �ȴ�У׼���*/   
     while(ADC_GetCalibrationStatus(ADC1));    
     /* ����û�в����ⲿ����������ʹ��������� ADC ת�� */     
     ADC_SoftwareStartConvCmd(ADC1, ENABLE);    
}

void DMA_Configuration_ADC(void)
{
	 DMA_InitTypeDef DMA_InitStructure;    
         
     /*DMA channel1 configuration*/   
     DMA_DeInit(DMA1_Channel1); //DAM1 ͨ��1    
     DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;  /*ADCת�������ַ*/ //�����ַ
     DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue[0]; /*�ڴ��ַ�����ܴ����adc���*/
	     
     DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Դ   
     DMA_InitStructure.DMA_BufferSize = 3; //���ݿ��    
     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*�����ַ�̶�*///DMA_PeripheralInc_Enable ��ַ����   
     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /*�ڴ��ַ�̶�*/    
     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //����    
     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;    
     DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //ѭ������    
     DMA_InitStructure.DMA_Priority = DMA_Priority_High;    
     DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;    
     DMA_Init(DMA1_Channel1, &DMA_InitStructure);    
     /* Enable DMA channel1 */       
	 DMA_Cmd(DMA1_Channel1, ENABLE);		
}

void Adc_Init(void)
{
	RCC_Configuration_ADC();
	GPIO_Configuration_ADC();
	DMA_Configuration_ADC();
	ADC1_Configuration_DMA();
}




