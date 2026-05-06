#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "PID.h"
#include "Encoder.h"
#include "Motor.h"
#include "ultrasonic.h"
#include "Sensor.h"
#include "Position_PID.h"
#include "usart.h"
extern uint16_t result[9];
float sudu1 = 0;
float sudu2 = 0;
extern float pwm1;
int main()
{
	OLED_Init();
	PWM_Init();
    Encoder1_Init();
   Encoder2_Init();
    Motor_Init();	
    PID_Init(&pid_motor1);
   	PID_Init(&pid_motor2);
	Position_PID_Init();
	Motor1_Control_Forwald(); 
    Motor2_Control_Forwald();
    Sensor_Init();
	Serial_Init();
    while (1)
    {
		
		
		Binarization_Init();
	int16_t a =	Power();
		int32_t add = error_calculation();
		OLED_ShowSignedNum(3,1,add,5);
		 OLED_ShowNum(1,7,TIM_GetCounter(TIM2),5);
         OLED_ShowNum(1,1,sudujisuan1,5);
	OLED_ShowNum(2,1,sudujisuan2,5);
			OLED_ShowNum(4,1,pwm1,5);
 Delay_ms(8);
		
		
sudu1 = 300 + add;	
sudu2 = 300 - add;

    }    	
}
	
