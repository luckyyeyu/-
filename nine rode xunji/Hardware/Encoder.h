#ifndef __ENCODER_H
#define __ENCODER_H
void Encoder1_Init(void);
float Motor_speed1(void);
void Encoder2_Init(void);
float Motor_speed2(void);
extern PID_Controller pid_motor1;
extern PID_Controller pid_motor2;

extern float sudujisuan1;
extern float sudujisuan2;
extern uint32_t cha_time;
extern int32_t  cha_count1;
extern int32_t  cha_count2;
#endif
