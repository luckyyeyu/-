#include "stm32f10x.h"                  // Device header
#include "PID.h"


PID_Controller pid_motor1;
PID_Controller pid_motor2;

static int32_t Last_current_count1 = 0;
static int32_t Last_current_count2 = 0;
extern float sudu1;
extern float sudu2;
void Encoder1_Init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=65536-1; //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=1-1;  //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel= TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter= 0x6;
	TIM_ICInitStructure.TIM_ICPolarity= TIM_ICPolarity_Rising;
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel= TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter= 0x6;
	TIM_ICInitStructure.TIM_ICPolarity= TIM_ICPolarity_Rising;
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_Cmd(TIM2,ENABLE);
}
void Encoder2_Init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM4);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=65536-1; //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=1-1;  //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter =0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel= TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter= 0x6;
	TIM_ICInitStructure.TIM_ICPolarity= TIM_ICPolarity_Rising;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel= TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter= 0x6;
	TIM_ICInitStructure.TIM_ICPolarity= TIM_ICPolarity_Rising;
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_Cmd(TIM4,ENABLE);
}

float sudujisuan1 = 0;
float sudujisuan2 = 0;

int32_t  cha_count1 = 0;
int32_t  cha_count2 = 0;

float Motor_speed1(void)
{	
	
	int32_t current_count = TIM_GetCounter(TIM2);	 
	  cha_count1 = current_count - Last_current_count1;
	 if (cha_count1 > 32768) {
        cha_count1 -= 65536;   
    } else if (cha_count1 < -32768) {
        cha_count1 += 65536;      
    }	
	
    Last_current_count1 = current_count;		
	
	 sudujisuan1 = ((float)cha_count1/440.0f)*(6000.0f);
	
	return sudujisuan1;
}

float Motor_speed2(void)
{	
	
	int32_t current_count = TIM_GetCounter(TIM4);	 
	  cha_count2 = current_count - Last_current_count2;
	 if (cha_count2 > 32768) {
        cha_count2 -= 65536;   
    } else if (cha_count2 < -32768) {
        cha_count2 += 65536;      
    }	
    Last_current_count2 = current_count;		
	
	sudujisuan2 = ((float)cha_count2/440.0f)*(6000.0f);
	
	return sudujisuan2;
}
float pwm1 = 0;
float pwm2 = 0;
void TIM3_IRQHandler(void)
{
if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
{
  static uint8_t ms_count = 0;
	ms_count++;
	if(ms_count>=10)
	{
	ms_count = 0;
	Motor_speed1();		
	Motor_speed2();
		if(sudu1<0)
		{
	     pwm1 = PID_Calculate(&pid_motor1, -sudu1, sudujisuan1);
		}
		else
		{
		 pwm1 = PID_Calculate(&pid_motor1, sudu1, sudujisuan1);
		}
		if(sudu2<0)
		{
             pwm2 = PID_Calculate(&pid_motor2, -sudu2, sudujisuan2);
		} 
		else
		{
            pwm2 = PID_Calculate(&pid_motor2, sudu2, sudujisuan2); 
		}
            TIM_SetCompare3(TIM3, pwm1);
            TIM_SetCompare4(TIM3, pwm2);
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
}




