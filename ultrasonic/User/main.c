#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "timer.h"
#include "usart.h"
#include "OLED.h"
#include "PWM.h"
#include "Motor.h"
#include "ultrasonic.h"
extern uint16_t Length;
int main(void)
{   
	TIM2_Init();
    OLED_Init();    
    PWM_Init();
    Motor_Init();
    Motor_Forward();
    ultrao_Init();
	
    while (1)
    {
	   send_information();
		Delay_ms(500);
		OLED_ShowNum(1,1,Length,5);
		OLED_ShowNum(2,1,TIM_GetCapture1(TIM3),5);
		if(Length<=20)
		{
		Motor_Back();
			TIM_SetCompare1(TIM3,300);
		}
		else if(Length>20&&Length<25)
		{
		Motor_Stop();
		}
		else if(Length>=25)
		{
		Motor_Forward();
		TIM_SetCompare1(TIM3,500);
		
		}
}
}

