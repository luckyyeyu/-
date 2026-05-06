#include "stm32f10x.h"                  // Device header
#include "Position_PID.h"
PositionPID_TypeDef position_pid;
void Position_PID_Init(void)
{

	position_pid.kp = 23.0f;         
	position_pid.ki = 0.5f;        
	position_pid.kd = 4.0f;         
	position_pid.integral = 0.0f;
	position_pid.last_error = 0.0f;
	position_pid.max_output = 190;
	
}
float proportional = 0;
float i_term = 0;
float d_term = 0; 
float error = 0;
float output = 0;
int32_t int_output = 0;
int32_t Position_PID_Calculate(float position_error)
{
	float output = 0.0f;
	

	float proportional = position_pid.kp * position_error;
	
	position_pid.integral += position_error;
	float max_integral = position_pid.max_output / position_pid.ki;
	if (position_pid.integral > max_integral) {
			position_pid.integral = max_integral;
	} else if (position_pid.integral < -max_integral) {
			position_pid.integral = -max_integral;
	}
	float integral_term = position_pid.ki * position_pid.integral;
	
	float derivative = position_error - position_pid.last_error;
	float derivative_term = position_pid.kd * derivative;
	
	// PID
	output = proportional + derivative_term;
	
	if (output > position_pid.max_output) {
			output = position_pid.max_output;
	} else if (output < -position_pid.max_output) {
			output = -position_pid.max_output;
	}
	
	position_pid.last_error = position_error;
	
	 int_output = output;

	return int_output;
}

