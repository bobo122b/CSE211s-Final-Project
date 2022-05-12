/*********************************************************************************************
// @file Microwave.c
// @author Abdelrahman Mostafa, Ahmed Hamdy, Ahmed Shafeek, Muhammed Emad, Abdelrahman Sameh
// @a      this file icludes main function which operates directly on the Tiva. it uses other 
//         project files included in the Microwave.h file
*********************************************************************************************/
#include "Microwave.h"

void SystemInit(void) {
    PLL_Init();             // configure PLL to use Tiva in 80MHZ mode
    SysTick_Init();         // initialize SysTick timer
    LCD_Init();             // initialize LCD pins
    Keypad_Init();          // initialize keypad pins
}

int main(void) {
    State state = IDLE;
    SystemInit();
    while(1) {

    }
}