#ifndef __TRACK_H__
#define __TRACK_H__

#include "stm32f10x.h"
#include "Motor.h"
#include "Servo.h"
#include "mySensor.h"
#include "attitude_solution.h" // 引入偏航角计算相关头文件

extern float Kp = 10.0f;
extern float Ki = 0.0f;
extern float Kd = 1.0f;
extern int16_t baseSpeed = 500; // 基础车速
extern float steerStep = 1.0f;  // 作举调整步长
extern float lastError = 0.0f;
extern float integral = 0.0f;

// 黑线跟踪主控制函数
void Track_Run(void);

#endif