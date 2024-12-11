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

	/*OLED显示*/
	OLED_ShowString(1, 1, "Pos:");
	Servo_SetAngle(0);

	Servo_SetAngle(50);

	ID = MPU6050_GetID();						   // 获取MPU6050的ID号
	MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ); // 获取MPU6050的数据

	while (1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ); // 获取MPU6050的数据
		sprintf(str, "yaw:%.2f", eulerAngle.yaw);
		OLED_ShowString(4, 1, str);
		uint8_t key = Key_GetNum();
		if (key == 1)
		{
			if (Angle < 180)
			{
				Angle += 10;
				Servo_SetAngle(Angle);
			}
		}
		else if (key == 2)
		{
			if (Angle > 0)
			{
				Angle -= 10;
				Servo_SetAngle(Angle);
			}
		}
		else if (key == 3)
		{
			speed += SPEED_INCREMENT;
			Motor_LeftSetSpeed(speed);
			Motor_RightSetSpeed(speed);
			OLED_ShowNum(3, 1, speed, 3);
		}
		else if (key == 4)
		{
			speed -= SPEED_INCREMENT;
			Motor_LeftSetSpeed(speed);
			Motor_RightSetSpeed(speed);
			OLED_ShowNum(3, 1, speed, 3);
		}
		else
		{
		}
		OLED_ShowNum(2, 1, Angle, 3);
		int16_t pos = mySensor_Read();
		OLED_ShowNum(1, 5, pos, 3);
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
