#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "PWM.h"

/**
 * 函    数：PWM初始化
 * 参    数：无
 * 返 回 值：无
 */
void PWM_Init_Servo(void)
{
	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  // 开启 TIM1 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启 GPIOA 的时钟

	/* GPIO 初始化 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  // 复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		  // PA11 引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出速度 50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  // 初始化 PA11

	/* 配置时钟源 */
	TIM_InternalClockConfig(TIM1); // TIM1 使用内部时钟

	/* 时基单元初始化 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				// 定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				// 自动重装载值（20ms，典型舵机周期）
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				// 预分频器，1 MHz 定时器频率
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;			// 重复计数器
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

	/* 主输出使能（MOE） */
	TIM_CtrlPWMOutputs(TIM1, ENABLE); // 使能高级定时器的主输出

	/* 输出比较初始化 */
	TIM_OCInitTypeDef TIM_OCInitStructure;						  // 定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);						  // 初始化结构体
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  // PWM 模式 1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  // 输出极性高
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;

	// 初始的 CCR 值（占空比）
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); // 配置 TIM1 通道 4

	/* TIM 使能 */
	TIM_Cmd(TIM1, ENABLE); // 使能 TIM1

	TIM_SetCompare4(TIM1, 0);
}

void PWM_Init_Motor()
{
	// 启用 GPIO 和 TIM3 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// 配置电机方向控制引脚为推挽输出
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	// 配置 PB1, PB0, PA7, PA6
	GPIO_InitStructure.GPIO_Pin = PWMB_OUT1 | PWMB_OUT2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PWMA_OUT3 | PWMA_OUT4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// // 配置 TIM3 的 PWM 输出引脚
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // TIM3_CH1 和 TIM3_CH2
	// GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 配置 TIM3 定时器
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;  // PWM 周期 (10kHz)
	TIM_TimeBaseStructure.TIM_Prescaler = 36 - 1; // 预分频器 72MHz/72 = 1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// 配置 TIM3 通道 1 和通道 2
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM 模式 1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0; // 默认占空比为 0
	// TIM_OC1Init(TIM3, &TIM_OCInitStructure); // TIM3_CH1
	// TIM_OC2Init(TIM3, &TIM_OCInitStructure); // TIM3_CH2
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); // TIM2_CH3
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); // TIM2_CH4
	TIM_OC3Init(TIM3, &TIM_OCInitStructure); // TIM3_CH3
	TIM_OC4Init(TIM3, &TIM_OCInitStructure); // TIM3_CH4

	// 启动 TIM3
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

/**
 * 函    数：PWM设置CCR
 * 参    数：Compare 要写入的CCR的值，范围：0~100
 * 返 回 值：无
 * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
 *           占空比Duty = CCR / (ARR + 1)
 */
// void PWM_SetCompare3(uint16_t Compare)
// {
// 	TIM_SetCompare3(TIM2, Compare); // 设置CCR3的值
// }
// /**
//  * 函    数：PWM设置CCR
//  * 参    数：Compare 要写入的CCR的值，范围：0~100
//  * 返 回 值：无
//  * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
//  *           占空比Duty = CCR / (ARR + 1)
//  */
// void PWM_SetCompare2(uint16_t Compare)
// {
// 	TIM_SetCompare4(TIM1, Compare); // 设置CCR2的值
// 	OLED_ShowNum(2, 1, Compare, 5);
// }

void PWM_SetCompare_Servo(uint16_t Compare)
{
	TIM_SetCompare4(TIM1, Compare);
}

void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM3, Compare);
}
void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM3, Compare);
}
// 注意这里引脚改变了,从原来的PA6,7改为PA2,3,服了队友了
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare3(TIM2, Compare);
}
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare4(TIM2, Compare);
}