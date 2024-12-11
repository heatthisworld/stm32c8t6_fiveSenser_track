#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "mySensor.h"
#include "Key.h"
#include "Motor.h"
#include "MPU6050.h"
#include "stdio.h"
#include "Timer.h"
#include "attitude_solution.h"
#include "Track.h"

// 调试时每次电机速度增量
#define SPEED_INCREMENT 100

uint8_t ID;						// 定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ; // 定义用于存放各个数据的变量

char str[64];

float Angle;

int main(void)
{
	int16_t speed = 300;
	/*模块初始化*/
	OLED_Init(); // OLED初始化
	Servo_Init();
	mySensor_Init(); // 传感器初始化
	Key_Init();		 // 按键初始化
	// Moter_Simple_Init(); // 简单电机初始化
	Motor_Init();
	MPU6050_Init(); // MPU6050初始化 	注意将IIC速率改高一点 200khz可以实现
	Timer_Init();
	LineTracking_Init();

	/*OLED显示*/
	// OLED_ShowString(3, 1, "Pos:");
	Servo_SetAngle(50);

	while (1)
	{
		LineTracking_Update();
	}
}
// 定时器中断函数，可以复制到使用它的地方
void TIM2_IRQHandler(void) // 10ms执行一次
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		ICM_getEulerianAngles(); // 获取陀螺仪数据并进行四元数解算
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
