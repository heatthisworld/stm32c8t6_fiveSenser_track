#ifndef __TRACK_H__
#define __TRACK_H__

#include "stm32f10x.h"

// 初始化函数
void LineTracking_Init(void);

// 黑线跟踪主控制函数
void LineTracking_Update(void);

#endif