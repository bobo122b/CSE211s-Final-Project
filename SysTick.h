#ifndef SYSTICK_H_
#define SYSTICK_H_
#include "tm4c123gh6pm.h"

// Functions definitions of SysTick
void SysTick_Init(void);
void SysTick_Wait(uint32_t delay);
void SysTick_Wait1ms(uint32_t delayms);
void SysTick_Wait1us(uint32_t delayus);
void PLL_Init(void);

#endif // !SYSTICK_H_