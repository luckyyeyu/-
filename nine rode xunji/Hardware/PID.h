#ifndef __PID_H
#define __PID_H                 
typedef struct {
    float Kp, Ki, Kd;
    float last_error;
    float error_sum;      
    float max_error_sum;
    float max_output;
} PID_Controller;


float PID_Calculate(PID_Controller* pid,float target,float current);
void PID_Init(PID_Controller* pid);
uint16_t Motor1_speed(void);
uint16_t Motor2_speed(void);


#endif
