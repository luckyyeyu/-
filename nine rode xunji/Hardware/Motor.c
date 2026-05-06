#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "PID.h"

void Motor_Init(void)
{
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;//声明结构体变量
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//分成员赋值
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void Motor1_Control_Forwald(void)
{
GPIO_SetBits(GPIOB,GPIO_Pin_13);
GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}
void Motor1_Control_Back(void)
{                                                                               
GPIO_ResetBits(GPIOB,GPIO_Pin_13);
GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
void Motor2_Control_Forwald(void)
{
GPIO_SetBits(GPIOB,GPIO_Pin_11);
GPIO_ResetBits(GPIOB,GPIO_Pin_10);                                                                   
}
void Motor2_Control_Back(void)
{
GPIO_ResetBits(GPIOB,GPIO_Pin_11);
GPIO_SetBits(GPIOB,GPIO_Pin_10);
}
void Turn_right(void)
{
Motor2_Control_Forwald();
//GPIO_ResetBits(GPIOB,GPIO_Pin_13);
//GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	
}
void Turn_left(void)
{
//GPIO_ResetBits(GPIOB,GPIO_Pin_11);
//GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	Motor1_Control_Forwald();
}
void Forward(void)
{
GPIO_SetBits(GPIOB,GPIO_Pin_13);
GPIO_ResetBits(GPIOB,GPIO_Pin_12);
GPIO_SetBits(GPIOB,GPIO_Pin_11);
GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}
