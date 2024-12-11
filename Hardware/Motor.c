#include "stm32f10x.h" // Device header
#include "PWM.h"

/**
 * 函    数：直流电机初始化
 * 参    数：无
 * 返 回 值：无
 */
void Motor_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 开启GPIOB的时钟

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA4和PA5引脚初始化为推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 将PB0和PB1引脚初始化为推挽输出
	PWM_Init_Motor();					   // 初始化直流电机的底层PWM
}
void Motor_LeftSetSpeed(int16_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		// PWM_SetCompare3(Speed);
		PWM_SetCompare4(Speed);
	}
	else
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		// PWM_SetCompare3(-Speed);
		PWM_SetCompare3(-Speed);
	}
}
void Motor_RightSetSpeed(int16_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		// PWM_SetCompare4(Speed * 0.93);
		PWM_SetCompare2(Speed * 0.93);
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		// PWM_SetCompare4(-Speed * 0.93);
		PWM_SetCompare1(-Speed * 0.93);
	}
}
