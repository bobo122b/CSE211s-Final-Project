#ifndef MICROWAVE_H_
#define MICROWAVE_H_

// Adding all needed files to be included in one file
#include "tm4c123gh6pm.h"
#include "lcd.h"
#include "keypad.h"
#include "SysTick.h"
#include "switches.h"

// definitions of used macros in main function
#define doorClosed !(SW3_Pressed)
#define Start SW2_Pressed
#define Pause SW1_Pressed 

#define not_valid_num(n) (n=='A'||n=='B'||n=='C'||n=='D'||n=='*'||n=='#')
#define not_valid_input(n) (n!='A' && n!='B' && n!='C' && n!='D')
#define fill_array(arr1,arr2) arr1[0]=arr2[0]; arr1[1]=arr2[1]; arr1[2]=arr2[2]; arr1[3]=arr2[3]; arr1[4]=arr2[4] ; arr1[5]=0

/*!
 * @typedef     State
 * @c           IDLE    Ready for any input, door is closed.
 *                      can go to INPUT if any input occurs.
 * @c           INPUT   Taking input from user, door is closed.
 *                      can go to IDLE if user cancels.
 *                      can go to COOKING if user accepts specified input.
 * @c           COOKING Took input from user and accepted, door is closed.
 *                      can go to PAUSED if any interrupt occurs.
 *                      can go to IDLE if cooking is finished.
 * @c           PAUSED  Door is open, or was cooking and user clicked on pause button.
 *                      can go to COOKING if user continued (timer must be saved).
 *                      can go to IDLE if user clicked on end.
*/
typedef enum {IDLE, INPUT, COOKING, PAUSED} State;

// Functions definitions

/*!
 * @fn SystemInit
 * @brief This function includes all initializations in order
*/
void SystemInit(void);

/* INPUTS DESCRIPTION:
// A -> Popcorn -> display PopCorn and coock for 1 minute
// B -> beef    -> enter kilos from 1 to 9 -> 0.5min per kilo
// C -> chicken -> enter kilos from 1 to 9 -> 0.2min per kilo
// D -> take input time from user from right to left (00:00)
//      sw1(pf4) -> clear LCD 
//      sw2(pf0) -> start cooking
// sw3(ext) -> door latch
*/

/* CONDITIONS
// sw3(ext) -> pressed = door open -> no cooking
                         if cooking -> pause (keep timer on screen)
// sw2(pf0) -> pressed = start operation
//                       if paused -> continue cooking
// sw1(pf4) -> pressed = if first time -> pause (keep timer on screen)
//                       if second     -> clear time, stop cooking
// if cooking -> all leds on
// if finished cooking -> all leds flash 3 times with buzzer
*/
#endif // !MICROWAVE_H_
