#include "stm32f10x.h"                  // Device header
#include "PID.h"
void PID_Init(PID_Controller* pid){

	pid->Kp = 2.0;
	pid->Ki = 1.5;
	pid->Kd = 0.1;
	pid->last_error = 0;
	pid->error_sum = 0;
	pid->max_error_sum = 950;
	pid->max_output = 950;
}
float PID_Calculate(PID_Controller* pid,float target,float current)//速度转化为PWM
{
	//误差计算
float error = target - current;
	//p相
 
	float P_term = pid->Kp*error;
	//i相
          pid->error_sum += error;
	if(pid->error_sum > pid->max_error_sum)
	{
	pid->error_sum = pid->max_error_sum;
	}
    if(pid->error_sum < -pid->max_error_sum)
	{
	pid->error_sum = -pid->max_error_sum;
	}
	float i_term = pid->Ki*pid->error_sum;
	//d相
	float d_term = pid->Kd*(error - pid->last_error);
	pid->last_error = error;
	 float output = P_term + i_term + d_term;
	if(output >pid->max_output)
	{
	output = pid->max_output;
	}
	if(output <-pid->max_output)
	{
	output = -pid->max_output;
	}
	return output;//计算后返回PWM，回到main.c
}




















