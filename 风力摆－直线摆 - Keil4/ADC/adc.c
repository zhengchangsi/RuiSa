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

/* 函数名：ADC1_Mode_Config   
 * 描述  ：配置 ADC1 的工作模式为 dma 模式   
 * 输入  : 无   
 * 输出  ：无   
 * 调用  ：内部调用   
 */   
void ADC1_Configuration_DMA(void)   // unsigned char channel
{    
     ADC_InitTypeDef ADC_InitStructure;
     /* ADC1 configuration */     
     ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  /*独立 ADC 模式*/    
     ADC_InitStructure.ADC_ScanConvMode = ENABLE ;   /*使能扫描模式，扫描模式用于多通道采集*/    
     ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  /*开启连续转换模式，即不停地进行 ADC 转换*/    
     ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; /*不使用外部触发转换*/    
     ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  /*采集数据右对齐*/    
     ADC_InitStructure.ADC_NbrOfChannel = 3;     /*要转换的通道数目 1*/    
     ADC_Init(ADC1, &ADC_InitStructure);    
       
     /*配置 ADC 时钟，为 PCLK2 的 8 分频，即 9MHz*/ 
     RCC_ADCCLKConfig(RCC_PCLK2_Div8);  

//    if (channel == LEFT_CHANNEL) /*配置 ADC1 的通道 4 为 55.5 个采样周期 */     
     //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);
//    else if (channel == RIGHT_CHANNEL) /*配置 ADC1 的通道 5 为 55.5 个采样周期 */ 
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_28Cycles5); 
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_28Cycles5); 

	   
     /* Enable ADC1 DMA */   
     ADC_DMACmd(ADC1, ENABLE);   
     /* Enable ADC1 */   
     ADC_Cmd(ADC1, ENABLE);    
     /*复位校准寄存器 */       
     ADC_ResetCalibration(ADC1);   
     /*等待校准寄存器复位完成 */   
     while(ADC_GetResetCalibrationStatus(ADC1));    
     /* ADC 校准 */   
     ADC_StartCalibration(ADC1);    
     /* 等待校准完成*/   
     while(ADC_GetCalibrationStatus(ADC1));    
     /* 由于没有采用外部触发，所以使用软件触发 ADC 转换 */     
     ADC_SoftwareStartConvCmd(ADC1, ENABLE);    
}

void DMA_Configuration_ADC(void)
{
	 DMA_InitTypeDef DMA_InitStructure;    
         
     /*DMA channel1 configuration*/   
     DMA_DeInit(DMA1_Channel1); //DAM1 通道1    
     DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;  /*ADC转换结果地址*/ //外设地址
     DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue[0]; /*内存地址，接受传输的adc结果*/
	     
     DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为数据源   
     DMA_InitStructure.DMA_BufferSize = 3; //数据宽度    
     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*外设地址固定*///DMA_PeripheralInc_Enable 地址递增   
     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /*内存地址固定*/    
     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //半字    
     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;    
     DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //循环传输    
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




