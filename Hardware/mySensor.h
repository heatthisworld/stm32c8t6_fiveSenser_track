#ifndef __MYSENSOR_H_
#define __MYSENSOR_H_
#include "stm32f10x.h"

// 红外感应器引脚定义
#define IR_SENSOR1_PIN GPIO_Pin_12
#define IR_SENSOR2_PIN GPIO_Pin_13
#define IR_SENSOR3_PIN GPIO_Pin_14
#define IR_SENSOR4_PIN GPIO_Pin_15
#define IR_SENSOR5_PIN GPIO_Pin_8

void mySensor_Init(void);
uint8_t mySensor_Read(void);

#endif
