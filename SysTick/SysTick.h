#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h"
#define Delay_ms(x)		Delay_us(1000*x)
void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
void SysTick_Enable(void);
#endif /* __SYSTICK_H */
