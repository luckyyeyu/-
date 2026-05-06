#ifndef __POSITION_PID_H                
#define __POSITION_PID_H
typedef struct {
    float kp;           
    float ki;           
    float kd;           
    float integral;     
    float last_error;   
    float max_output;   
} PositionPID_TypeDef;

void Position_PID_Init(void);
extern PositionPID_TypeDef position_pid;

int32_t Position_PID_Calculate(float position_error);

#endif


