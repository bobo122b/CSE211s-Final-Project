/*************************************************************************************************
 *!
 * @file 		Timer.c
 * @authors  	Muhammed Emad, Abdelrahman Sameh, Abdelrahman Mostafa, Ahmed Shafeek, Ahmed Hamdy
 * @brief		contains the functions that define how to make the timer string
**************************************************************************************************/
#include "Microwave.h"

void timing_string(char Weight, char keypadInput)
{
	float temp, min;
	int seconds;
					
	if      (keypadInput=='B') temp = 0.5;		// for beef 	-> 0.5 minute for each kilo
	else if (keypadInput=='C') temp = 0.2;		// for chicken 	-> 0.2 minute for each kilo
						
	min 	= (Weight-'0') * temp;				
	seconds = (int)(min * 60) %60;				// mapping the seconds from 0 to 59
	min 	= (int)(min);
	cookingTime[4] = ((int)seconds %10) +'0';	// mapping 1  * sec to 0 to 9
	cookingTime[3] = (seconds)/10 +'0';			// mapping 10 * sec to 0 to 6
	cookingTime[2] = ':';
	cookingTime[1] = min+'0';
	cookingTime[0] = '0';
}

