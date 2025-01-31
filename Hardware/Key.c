#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"

/**
 * 函    数：按键初始化
 * 参    数：无
 * 返 回 值：无
 */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 开启GPIOC的时钟

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PB0和PB5引脚初始化为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
	GPIO_Init(GPIOC, &GPIO_InitStructure); // 将PC14和PC15引脚初始化为上拉输入
}

/**
 * 函    数：按键获取键码
 * 参    数：无
 * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
 * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
 */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0; // 定义变量，默认键码值为0

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0) // 读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20); // 延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)
			;		  // 等待按键松手
		Delay_ms(20); // 延时消抖
		KeyNum = 1;	  // 置键码为2
	}

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) // 读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20); // 延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
			;		  // 等待按键松手
		Delay_ms(20); // 延时消抖
		KeyNum = 2;	  // 置键码为1
	}

	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 0) // 读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20); // 延时消抖
		while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 0)
			;		  // 等待按键松手
		Delay_ms(20); // 延时消抖
		KeyNum = 3;	  // 置键码为4
	}

	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 0) // 读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20); // 延时消抖
		while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 0)
			;		  // 等待按键松手
		Delay_ms(20); // 延时消抖
		KeyNum = 4;	  // 置键码为3
	}

	return KeyNum; // 返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}

uint8_t Key_GetNum_Advance()
{
	uint8_t KeyNum = 0;								   // 定义变量，默认键码值为0
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0) // 读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20); // 延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)
		{
			if (KeyNum < !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) + !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) * 2 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) * 4 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) * 8)
				KeyNum = !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) + !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) * 2 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) * 4 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) * 8;
		}
		Delay_ms(20); // 延时消抖
		return KeyNum;
	}

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) // 读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20); // 延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			if (KeyNum < !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) + !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) * 2 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) * 4 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) * 8)
				KeyNum = !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) + !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) * 2 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) * 4 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) * 8;
		}
		Delay_ms(20); // 延时消抖
		return KeyNum;
	}

	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 0) // 读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20); // 延时消抖
		while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 0)
		{
			if (KeyNum < !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) + !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) * 2 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) * 4 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) * 8)
				KeyNum = !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) + !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) * 2 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) * 4 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) * 8;
		}

		Delay_ms(20); // 延时消抖
		return KeyNum;
	}

	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 0) // 读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20); // 延时消抖
		while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 0)
		{
			if (KeyNum < !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) + !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) * 2 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) * 4 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) * 8)
				KeyNum = !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) + !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) * 2 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) * 4 + !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) * 8;
		} // 等待按键松手
		Delay_ms(20); // 延时消抖
		return KeyNum;
	}
	return KeyNum;
}
uint8_t Key_GetNum_gpt(void)
{
	uint8_t KeyNum = 0; // 定义变量，默认键码值为0
	static uint8_t lastKeyNum = 0;
	// 检查每个按键并更新KeyNum
	KeyNum |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) ? (1 << 0) : 0;
	KeyNum |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET) ? (1 << 1) : 0;
	KeyNum |= (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == Bit_RESET) ? (1 << 2) : 0;
	KeyNum |= (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == Bit_RESET) ? (1 << 3) : 0;

	// 如果没有按键被按下，直接返回
	if (KeyNum == 0)
	{
		return 0;
	}
	// 延时消抖
	Delay_ms(20);

	// 等待所有被按下的按键释放
	while ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) ||
		   (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET) ||
		   (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == Bit_RESET) ||
		   (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == Bit_RESET))
	{
		// 检查每个按键并更新KeyNum
		KeyNum |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) ? (1 << 0) : 0;
		KeyNum |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET) ? (1 << 1) : 0;
		KeyNum |= (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == Bit_RESET) ? (1 << 2) : 0;
		KeyNum |= (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == Bit_RESET) ? (1 << 3) : 0;
		lastKeyNum = KeyNum;
		if (KeyNum > lastKeyNum)
		{
			Delay_ms(20);
			// 检查每个按键并更新KeyNum
			KeyNum |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) ? (1 << 0) : 0;
			KeyNum |= (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET) ? (1 << 1) : 0;
			KeyNum |= (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == Bit_RESET) ? (1 << 2) : 0;
			KeyNum |= (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == Bit_RESET) ? (1 << 3) : 0;
			lastKeyNum = KeyNum;
		}
	}

	// 再次延时消抖
	Delay_ms(20);

	// 返回按键码
	return lastKeyNum;
}
