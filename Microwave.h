#ifndef MICROWAVE_H_
#define MICROWAVE_H_

/*!
 * @typedef     State
 * @constant    IDLE    Ready for any input, door is closed.
 *                      can go to INPUT if any input occurs.
 * @constant    INPUT   Taking input from user, door is closed.
 *                      can go to IDLE if user cancels.
 *                      can go to COOKING if user accepts specified input.
 * @constant    COOKING Took input from user and accepted, door is closed.
 *                      can go to PAUSED if any interrupt occurs.
 *                      can go to IDLE if cooking is finished.
 * @constant    PAUSED  Door is open, or was cooking and user clicked on pause button.
 *                      can go to COOKING if user continued (timer must be saved).
 *                      can go to IDLE if user clicked on end.
*/
typedef enum {IDLE, INPUT, COOKING, PAUSED} State;

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
