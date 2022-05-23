#ifndef STEPPER_H
#define STEPPER_H

/*!
 * @fn      StepperInit
 * @brief   Initializes Stepper Motor pins 
*/

void StepperInit(void);

/*!
 * @fn      RotorStart
 * @brief   Starts the motor while cooking && handles the lcd countdown
*/

void RotorStart(void);

#endif
