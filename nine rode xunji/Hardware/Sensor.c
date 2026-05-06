#include "stm32f10x.h"                  // Device header
#include "Position_PID.h"
#include "Delay.h"
uint16_t Value[9];
uint16_t result[9];
extern PositionPID_TypeDef position_pid;
void Sensor_Init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;//声明结构体变量
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//分成员赋值
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
float Sensor_Num = 0;
void Binarization_Init(void)
{ 
	Sensor_Num = 0.0f;
	result[0] = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == SET)? 0:1; 
	result[1] = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == SET)? 0:1; 
	result[2] = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == SET)? 0:1; 
	result[3] = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) == SET)? 0:1; 
    result[4] = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == SET)? 0:1; 
	result[5] = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == SET)? 0:1; 
	result[6] = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == SET)? 0:1; 
	result[7] = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == SET)? 0:1; 
	result[8] = (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == SET)? 0:1; 
	for(int i = 0;i<=8;i++)
	{
	if(result[i] == 1)
	{
	Sensor_Num +=1.0f;
	}
	}
	
}
int16_t arr[9] = {-8,-5,-3,-2,0,2,3,5,8};
int16_t power = 0;
int16_t Power(void)
{
	 power = 0;
for(int i = 0;i<=8;i++)
	{
	 power += result[i]*arr[i];		
	}
	return power;
}
float Sensor_error = 0;
float  out = 0 ;
float error_calculation(void)
{
	if(Sensor_Num!=0)
	{
   Sensor_error = power/Sensor_Num;
	}
	return out = Position_PID_Calculate(Sensor_error);
}

