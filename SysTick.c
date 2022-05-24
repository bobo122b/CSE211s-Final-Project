/*************************************************************************************************
 *!
 * @file 		SysTick.c
 * @authors  	Abdelrahman Sameh, Abdelrahman Mostafa, Ahmed Shafeek, Ahmed Hamdy, Muhammed Emad 
 * @brief		contains the functions that recognize and define SysTick timer and its use
**************************************************************************************************/
#include "Microwave.h"

void PLL_Init(void){
	// 0) Use RCC2
	SYSCTL_RCC2_R |= 0x80000000; // USERCC2
	// 1) bypass PLL while initializing
	SYSCTL_RCC2_R |= 0x00000800; // BYPASS2, PLL bypass
	// 2) select the crystal value and oscillator source
	SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) // clear bits 10-6
	+ 0x00000540; // 10101, configure for 16 MHz crystal
	SYSCTL_RCC2_R &= ~0x00000070; // configure for main oscillator source
	// 3) activate PLL by clearing PWRDN
	SYSCTL_RCC2_R &= ~0x00002000;
	// 4) set the desired system divider
	SYSCTL_RCC2_R |= 0x40000000; // use 400 MHz PLL
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000)+(SYSDIV2<<22); // 80 MHz
	// 5) wait for the PLL to lock by polling PLLLRIS
	while((SYSCTL_RIS_R&0x00000040)==0){}; // wait for PLLRIS bit
	// 6) enable use of PLL by clearing BYPASS
	SYSCTL_RCC2_R &= ~0x00000800;
}

void SysTick_Init(void) {
	NVIC_ST_CTRL_R    = 0;				// disable SysTick timer
	NVIC_ST_RELOAD_R  = 0x00FFFFFF;		// maximum value when initialized
	NVIC_ST_CURRENT_R = 0;				// to apply changes of reload
	NVIC_ST_CTRL_R    = 0X00000005;  	// Enable clk, Counter, No Interrupt
}

void SysTick_Wait(uint32_t delay) {
	NVIC_ST_RELOAD_R = delay - 1;
	NVIC_ST_CURRENT_R = 0;
	while ((NVIC_ST_CTRL_R&0x00010000)==0) {}
}

void SysTick_Wait1ms(uint32_t delayms) {
	unsigned long i;
	for (i = 0; i < delayms; i++)
		SysTick_Wait(80000);			// will delay 1ms
}

void SysTick_Wait1us(uint32_t delayus) {
	unsigned long i;
	for (i = 0; i < delayus; i++)
		SysTick_Wait(80);				// will delay 1us
}
