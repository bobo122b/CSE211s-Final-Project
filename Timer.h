/*************************************************************************************************
 *!
 * @file 		Timer.h
 * @authors  	Muhammed Emad, Abdelrahman Sameh, Abdelrahman Mostafa, Ahmed Shafeek, Ahmed Hamdy
 * @brief		contains the functions that define how to make the timer string
**************************************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

/*!
 * @fn      timing_string
 * @brief   modifies the cooking time according to weight and kind of input
 * @param   Weight      the weight of the input in kilos
 * @param   keypadInput the input indicated by user (defines whether to choose beef or chicken)
*/
void timing_string(char Weight, char Input); 

#endif // !TIMER_H_
