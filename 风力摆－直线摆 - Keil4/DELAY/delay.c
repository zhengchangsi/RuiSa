#include "delay.h"
#include "sys.h"
  					    

//延时nus
//nus为要延时的us数.		    								   

void delay_us(uint16_t n)
{
	uint16_t j, i;
	for(i=0;i<n;i++)
	{
		for(j=0;j<1;j++)
		{
			;
		}
	}
}	


void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	
	while(nms--){
		temp = 1000;
		while(temp--);
	}  
} 
































