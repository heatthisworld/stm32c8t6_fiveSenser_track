#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "mySensor.h"
#include "Key.h"
// #include "Moter_Simple.h"
#include "Motor.h"

// 调试时每次电机速度增量
#define SPEED_INCREMENT 10

float Angle;

int main(void)
{
	int16_t speed = 0;
	/*模块初始化*/
	OLED_Init(); // OLED初始化
	Servo_Init();
	mySensor_Init(); // 传感器初始化
	Key_Init();		 // 按键初始化
	// Moter_Simple_Init(); // 简单电机初始化
	Motor_Init();

	/*OLED显示*/
	OLED_ShowString(1, 1, "Pos:");
	Servo_SetAngle(0);

	Servo_SetAngle(50);

	while (1)
	{
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
