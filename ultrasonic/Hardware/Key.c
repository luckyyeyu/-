#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
extern uint16_t compare;
void Key_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef  GPIO_InitAtructure;
	GPIO_InitAtructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitAtructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitAtructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitAtructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);
	
    EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	;
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
    NVIC_Init(&NVIC_InitStructure);
}

 
void EXTI1_IRQHandler(void)
{
	
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
		 for(volatile int i = 0; i < 6000; i++);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
		{
     if(compare<1000)
	 {
		 compare +=100;
	 }
	 else
	 {
	 compare = 1000;
	 }
		}
		
	}
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
 uint8_t state = 0;
	
void EXTI15_10_IRQHandler(void)
{
if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
		 for(volatile int i = 0; i < 6000; i++);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0)
		{
     if(compare>0)
	 {
		 compare -=100;
	 }
	 else
	 {
	 compare = 0;
	 }
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
		
	}
        if(EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
		 for(volatile int i = 0; i < 6000; i++);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
		{
     
		state = !state;
			(state)? Motor_Back():Motor_Forward();
			
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
		
	}
    }
 





