#ifndef __TRACK_H__
#define __TRACK_H__

#include "stm32f10x.h"
#include "Motor.h"
#include "Servo.h"
#include "mySensor.h"
#include "attitude_solution.h" // 引入偏航角计算相关头文件
#include "OLED.h"
#include "Delay.h"

extern float Kp;
extern float Ki;
extern float Kd;
extern int16_t baseSpeed; // 基础车速
extern float steerStep;   // 作举调整步长
extern float lastError;
extern float integral;

// 黑线跟踪主控制函数
void Track_Run(void);

#endif