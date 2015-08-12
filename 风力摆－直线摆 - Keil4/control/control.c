#include "stm32f10x.h"
#include "control.h"
#include "timer.h"

void Control_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //用到PA13所以需要关闭jtag和sw
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM3_CH1  TIM3_CH2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6; //TIM3_CH1  TIM3_CH2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM3_CH1  TIM3_CH2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	PDout(5) = 1;
}


//方向控制 
void Direction_control(u8 dir, u8 motor_num)
{
	switch(motor_num){
		case MOTOR_NUM_A:{
				switch(dir){
					case MOTOR_DIR_FRONT:{
							//左正
							PDout(3) = 1;
							PDout(4) = 0;
							break;
					}
					case MOTOR_DIR_BACK:{
							//左反
							PDout(3) = 0;
							PDout(4) = 1;
							break;
					}
				case MOTOR_DIR_STOP:{
							//STOP
							PDout(3) = 1;
							PDout(4) = 1;
							break;
					}
				}
				break;		
		}
		case MOTOR_NUM_B:{
				switch(dir){
					case MOTOR_DIR_FRONT:{
					//右正
						PDout(6) = 1;
						PBout(9) = 0;
						break; 
					}
					case MOTOR_DIR_BACK:{
						//右反
						PDout(6) =  0;
						PBout(9) =  1;
						break;
					}
					case MOTOR_DIR_STOP:{
						//STOP
						PDout(6) = 1;
						PBout(9) = 1;
						break;
					}
				}
				break;		
		}
	}
}

void Motor_A_contro(uint16_t Motor_A_speed,u8 Motor_A_dir)
{
	if(Motor_A_speed+150 > TIM4_CONT)  Motor_A_speed = TIM4_CONT-150;
	
	TIM_SetCompare1(TIM4, Motor_A_speed+150);	
	Direction_control(Motor_A_dir,MOTOR_NUM_A);
}

void Motor_B_contro(uint16_t Motor_B_speed,u8 Motor_B_dir)
{
	if(Motor_B_speed> TIM4_CONT+150)  Motor_B_speed = TIM4_CONT-150;
	
	TIM_SetCompare3(TIM4, Motor_B_speed+150);
	Direction_control(Motor_B_dir,MOTOR_NUM_B);
}






void Motor_Speed_Ctrl(s16 Motor_speed, u8 Motor_Num)
{
	u8 dir;
	u16 Speed;
	
	switch (Motor_Num)
	{
		case MOTOR_NUM_A:
		{
			if (Motor_speed >= 0){Speed = Motor_speed; dir = MOTOR_DIR_FRONT;}
			else {Speed = 0;}
			
			if(Speed+150 > TIM4_CONT)  Speed = TIM4_CONT-150;
			TIM_SetCompare1(TIM4, Speed+150);	
			break;
		}
		case MOTOR_NUM_B:
		{
			if (Motor_speed >= 0){Speed = Motor_speed; dir = MOTOR_DIR_FRONT;}
			else {Speed = 0;}
			
			if(Speed+150 > TIM4_CONT)  Speed = TIM4_CONT-150;
			TIM_SetCompare3(TIM4, Speed+150);	
			break;
		}
		case MOTOR_NUM_C:
		{
			if (Motor_speed >= 0){Speed = Motor_speed; dir = MOTOR_DIR_FRONT;}
			else {Speed = 0;}
			
			if(Speed+150 > TIM4_CONT)  Speed = TIM4_CONT-150;
			TIM_SetCompare2(TIM4, Speed+150);	
			break;
		}
		case MOTOR_NUM_D:
		{
			if (Motor_speed >= 0){Speed = Motor_speed; dir = MOTOR_DIR_FRONT;}
			else {Speed = 0;}
			
			if(Speed+150 > TIM4_CONT)  Speed = TIM4_CONT-150;
			TIM_SetCompare4(TIM4, Speed+150);	
			break;
		}
		default:
			;
	}
}
	










