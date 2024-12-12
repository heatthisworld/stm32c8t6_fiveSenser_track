#include "mySensor.h"

// 五个红外传感器,传感器1-5的引脚为B12-B15 A8
/**
 * 函    数：按键初始化
 * 参    数：无
 * 返 回 值：无
 */
void mySensor_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOB的时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); // PB12,13,14,15引脚初始化为上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // PA8引脚初始化为上拉输入
}

/**
 * 函    数：读取红外传感器数据
 * 参    数：无
 * 返 回 值：无
 */
uint8_t mySensor_Read(void)
{
    uint16_t value = 0;
    value |= !GPIO_ReadInputDataBit(GPIOB, IR_SENSOR1_PIN) << 0;
    value |= !GPIO_ReadInputDataBit(GPIOB, IR_SENSOR2_PIN) << 1;
    value |= !GPIO_ReadInputDataBit(GPIOB, IR_SENSOR3_PIN) << 2;
    value |= !GPIO_ReadInputDataBit(GPIOB, IR_SENSOR4_PIN) << 3;
    value |= !GPIO_ReadInputDataBit(GPIOA, IR_SENSOR5_PIN) << 4;
    return value;
}
