/*************************************************************************************************
 *!
 * @file 		SysTick.h
 * @authors  	Abdelrahman Sameh, Abdelrahman Mostafa, Ahmed Shafeek, Ahmed Hamdy, Muhammed Emad 
 * @brief		contains the functions that recognize and define SysTick timer and its use
**************************************************************************************************/
#ifndef SYSTICK_H_
#define SYSTICK_H_
#include "tm4c123gh6pm.h"
#include "stdint.h"


#define SYSDIV2 4

// Functions definitions of SysTick

/*!
 * @fn         SysTick_Init
 * @brief      Used to initialize the systick timer
*/
void SysTick_Init(void);
void SysTick_Wait(uint32_t delay);
/*!
 * @fn         SysTick_Wait1ms
 * @brief	   this will take an input in milliseconds, and
 * 			   delay the system for that amount of milliseconds.
 * @param 	   delayms 	millisecond input
*/
void SysTick_Wait1ms(uint32_t delayms);
/*!
 * @fn         SysTick_Wait1us
 * @brief      this will take an input in microseconds, and
 * 			   delay the system for that amount of microseconds.
 * @param 	   delayms 	microsecond input
*/
void SysTick_Wait1us(uint32_t delayus);
/*!
 * @fn         PLL_Init
 * @brief      used to initialize PLL to apply 80 MHZ to tiva board
*/
void PLL_Init(void);

#endif // !SYSTICK_H_
