#include "stm32f10x.h"                  // Device header
#include "delay.h"


void ultrao_Init(void) 
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource9);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM1_Init();

}
uint16_t Length = 0;
void send_information(void)
{
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    Delay_us(2);
	
    GPIO_SetBits(GPIOA,GPIO_Pin_8);
    Delay_us(20);
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line9)!= RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == SET)
        {
            TIM_SetCounter(TIM1, 0);
            TIM_Cmd(TIM1, ENABLE);
        }
        else
        {
            TIM_Cmd(TIM1, DISABLE);
            uint16_t t = TIM_GetCounter(TIM1);
            Length = t*10 * 0.034f / 2.0f; 
        }
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}
