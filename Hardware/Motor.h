#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f10x.h"

// 初始化电机驱动模块
void Motor_Init(void);

// 电机控制功能
void Motor_RightSetSpeed(int16_t Speed);
void Motor_LeftSetSpeed(int16_t Speed);
void Motor_SetSpeed(int16_t LeftSpeed, int16_t RightSpeed);

#endif
