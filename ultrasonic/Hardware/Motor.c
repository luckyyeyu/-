#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启时钟

     GPIO_InitTypeDef GPIO_InitStructure;//声明结构体变量
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//分成员赋值
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化

GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2);
}
void Motor_Forward(void)
{
GPIO_SetBits(GPIOA,GPIO_Pin_4);
GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}
void Motor_Back(void)
{
GPIO_SetBits(GPIOA,GPIO_Pin_5);
GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}
void Motor_Stop(void)
{
GPIO_SetBits(GPIOA,GPIO_Pin_4);
GPIO_SetBits(GPIOA,GPIO_Pin_5);
}
