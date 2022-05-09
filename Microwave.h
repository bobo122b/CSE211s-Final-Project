#ifndef MICROWAVE_H_
#define MICROWAVE_H_

/*
//*****************************************************************************
// enum State
// IDLE:    Ready for any input, door is closed.
//          can go to INPUT if any input occurs.
//
// INPUT:   Taking input from user, door is closed.
//          can go to IDLE if user cancels.
//          can go to COOKING if user accepts specified input.
//
// COOKING: Took input from user and accepted, door is closed.
//          can go to PAUSED if any interrupt occurs.
//          can go to IDLE if cooking is finished.
//
// PAUSED:  Door is open, or was cooking and user clicked on pause button.
//          can go to COOKING if user continued (timer must be saved).
//          can go to IDLE if user clicked on end.
//*****************************************************************************
*/

typedef enum {IDLE, INPUT, COOKING, PAUSED} State;


#endif // !MICROWAVE_H_
