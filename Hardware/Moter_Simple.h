#ifndef __MOTER_SIMPLE_H
#define __MOTER_SIMPLE_H

#include "stm32f10x.h"

void Moter_Simple_Init(void);
void Moter_Simple_stop(void);
void Moter_Simple_Forward(void);
void Moter_Simple_Backward(void);

#endif