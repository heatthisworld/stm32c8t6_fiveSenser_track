#ifndef __PWM_H
#define __PWM_H

#define PWMB_OUT1 GPIO_Pin_1
#define PWMB_OUT2 GPIO_Pin_0
#define PWMA_OUT3 GPIO_Pin_3
#define PWMA_OUT4 GPIO_Pin_2

// void PWM_Init(void);
void PWM_Init_Motor(void);
void PWM_Init_Servo(void);
void PWM_SetCompare_Servo(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);
void PWM_SetCompare4(uint16_t Compare);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);

#endif
