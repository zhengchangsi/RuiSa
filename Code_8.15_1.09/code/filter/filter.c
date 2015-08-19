/**********************************************************************************************
** 
** File Name		 				: filter.c
** Version       	 			: V1.0
** Create Time 	      	: 2013.8.16
** File Description  		: 滤波算法整合
** Instruction		 			: 几种滤波算法
** Hardware Connection  :  None
**
**********************************************************************************************/

//================================================================================================//
#include "stm32f10x.h"
#include "filter.h"
#include "stdlib.h"

//================================================================================================//



/**********************************************************************************************
** Function Name         : Order()
** Function Description  : 排序算法
** Inlet Parameters      : *str 待排序数据缓存	num 数组元素个数
** Outlet Parameters     : None
** Instruction	         : 对数组中的元素按从小到大的方式排序
** Date			 : 2013.8.16
**---------------------------------------------------------------------------------------------
** Modifier		 : 
** Date			 :
***********************************************************************************************/

void Order(float *str , uint8_t num)	// 排序
{
  uint8_t i, j;
  float temp = 0;

//===================================================================================//	
  
  for(i = 0; i < num - 1; i++)				// 排序算法
  {
    for(j = i + 1;j < num; j++)
    {
      if(str[i] > str[j])
      {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
      }
    }
  }
}



/**********************************************************************************************
** Function Name         : Median_Filter()
** Function Description  : 中值滤波
** Inlet Parameters      : *Median_Str 采样缓存，呆排序数组
** Outlet Parameters     : 采样均值
** Instruction	         : 对数据缓存排序后，选取中间3个数据计算平均值，并返回
** Date			 : 2013.8.16
**---------------------------------------------------------------------------------------------
** Modifier		 : 
** Date			 :
***********************************************************************************************/

float Median_Filter(float *Median_Str)
{
	uint8_t i;
	float Median_Sum = 0;
	float Average_Value = 0;

	Order(Median_Str, Median_Sampling_Num);
	
	for(i = Median_Sampling_Num/2 - 1; i <= Median_Sampling_Num/2 + 1; i++)
	{
		Median_Sum += Median_Str[i]; 
	}
	
	Average_Value = Median_Sum / 3.0;
	
	
	return Average_Value;
}

