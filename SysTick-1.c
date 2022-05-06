#include "SysTick.h"
void SysTick_Init(void) {
	NVIC_ST_CTRL_R    = 0;
	NVIC_ST_RELOAD_R  = 0x00FFFFFF;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R    = 0X00000005;
}

void SysTick_Wait(uint32_t delay) {
	NVIC_ST_RELOAD_R = delay - 1;
	NVIC_ST_CURRENT_R = 0;
	while ((NVIC_ST_CTRL_R&0x00010000)==0) {}
}

void SysTick_Wait1ms(uint32_t delayms) {
	unsigned long i;
	for (i = 0; i < delayms; i++)
		SysTick_Wait(80000);
}

void SysTick_Wait1us(uint32_t delayus) {
	unsigned long i;
	for (i = 0; i < delayus; i++)
		SysTick_Wait(80);
}