#ifndef MICROWAVE_H_
#define MICROWAVE_H_

// Adding all needed files to be included in one file
#include "tm4c123gh6pm.h"
#include "lcd.h"
#include "keypad.h"
#include "SysTick.h"

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
