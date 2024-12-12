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

	/*OLED显示*/
	OLED_ShowString(1, 1, "Key:");
	Servo_SetAngle(50);
	// 假设你已经实现 OLED 显示功能
	OLED_ShowString(1, 1, "Kp: ");
	OLED_ShowNum(1, 5, Kp, 4);

	OLED_ShowString(2, 1, "Ki: ");
	OLED_ShowNum(2, 5, Ki * 100, 4); // 显示放大 100 倍的值

	OLED_ShowString(3, 1, "Kd: ");
	OLED_ShowNum(3, 5, Kd * 100, 4); // 显示放大 100 倍的值

	OLED_ShowString(4, 1, "vAdd: ");
	OLED_ShowNum(4, 8, baseSpeed, 4);
	while (1)
	{
		Track_Run();
		uint8_t key = Key_GetNum_gpt();
		if (key == 0)
			continue;
		OLED_ShowNum(1, 5, key, 2);
		if (key == 0x01)
		{				 // 按键1按下
			Kp += 10.0f; // 增加比例增益
			OLED_ShowString(1, 1, "Kp: ");
			OLED_ShowNum(1, 5, Kp, 4);
		}
		else if (key == 0x02)
		{				 // 按键2按下
			Kp -= 10.0f; // 减少比例增益
			OLED_ShowString(1, 1, "Kp: ");
			OLED_ShowNum(1, 5, Kp, 4);
		}
		else if (key == 0x03)
		{				// 按键1,2按下
			Ki += 1.0f; // 增加积分增益
			OLED_ShowString(2, 1, "Ki: ");
			OLED_ShowNum(2, 5, Ki * 100, 4); // 显示放大 100 倍的值
		}
		else if (key == 0x04)
		{				// 按键3按下
			Ki -= 1.0f; // 减少积分增益
			OLED_ShowString(2, 1, "Ki: ");
			OLED_ShowNum(2, 5, Ki * 100, 4); // 显示放大 100 倍的值
		}
		else if (key == 0x05)
		{				// 按键1和按键3都按下
			Kd += 1.0f; // 增加微分增益
			OLED_ShowString(3, 1, "Kd: ");
			OLED_ShowNum(3, 5, Kd * 100, 4); // 显示放大 100 倍的值
		}
		else if (key == 0x06)
		{				// 按键2和按键3都按下
			Kd -= 1.0f; // 减少微分增益
			OLED_ShowString(3, 1, "Kd: ");
			OLED_ShowNum(3, 5, Kd * 100, 4); // 显示放大 100 倍的值
		}
		else if (key == 0x07)
		{					 // 1,2、3按键都按下
			baseSpeed += 10; // 增加基础车速
		}
		else if (key == 0x08)
		{					 // 按键4按下
			baseSpeed -= 10; // 减少基础车速
		}
		else if (key == 0x09)
		{ // 按键1,4按下
			if (baseSpeed <= 300)
				baseSpeed += 50; // 增大舵机调整步长
			OLED_ShowString(4, 1, "vAdd: ");
			OLED_ShowNum(4, 5, baseSpeed, 4);
		}
		else if (key == 0x0A)
		{ // 按键2、4按下
			if (baseSpeed >= 50)
				baseSpeed -= 50; // 减小舵机调整步长
			OLED_ShowString(4, 1, "vAdd: ");
			OLED_ShowNum(4, 5, baseSpeed, 4);
		}
		else if (key = 11)
		{
			// 按键1、2、4都按下
			Motor_SetSpeed(0, 0); // 停止电机
			while (((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_SET) ||
					(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET) ||
					(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == Bit_SET) ||
					(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == Bit_SET)))
			{
				// 停止追踪,直到有按键按下
			}
		}
		else if (key == 12)
		{
			// 按键3、4都按下
			steerStep += 10; // 增加舵机调整步长
		}
		else if (key == 13)
		{
			// 按键1、3,4都按下
			steerStep -= 10; // 减少舵机调整步长
		}
		else if (key == 14)
		{
		}
		else if (key == 15)
		{
		}
		else if (key == 16)
		{
		}
		else
		{
		}
		// 按键松开后或者时间间隔较长时，可以实时显示参数值
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
