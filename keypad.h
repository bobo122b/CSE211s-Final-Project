/*************************************************************************************************
 *!
 * @file 		keypad.h
 * @authors  	Ahmed Hamdy, Ahmed Shafeek, Abdelrahman Mostafa, Muhammed Emad, Abdelrahman Sameh 
 * @brief		contains the keypad initialization and input functions
 *  
**************************************************************************************************/
#ifndef KEYPAD_H_
#define KEYPAD_H_

// Macros used in keypad.c

// checks which row was chosen, and returns its value in Code
// default is either set to 0 or 1 to assure change in comparisons if none was chosen
#define check(code, default) if(row == 0x01) code = map[0][col];\
else if(row == 0x02) code = map[1][col];\
else if(row == 0x04) code = map[2][col];\
else if(row == 0x08) code = map[3][col];\
else code = default

#define setColumn(col) GPIO_PORTC_DATA_R &= ~0xF0; /* reset all columns to zero */\
GPIO_PORTC_DATA_R |= col_select[col]               /* set only one column to loop on */

#define check_Row   (row = GPIO_PORTE_DATA_R & 0x0F)

// Cols from C4:C7 
// Rows from E0:E3

/*!
 * @fn      Keypad_Init
 * @brief   Initializes Keypad pins as inputs and outputs
*/
void Keypad_Init(void);

/*!
 * @fn      Keypad_getKey
 * @brief   scans the keypad looking for any immediate input
 * @return  returns character pressed on keypad, or '~' in the case
 *          of SW1, or 's' in the case of SW2.
*/
char Keypad_getKey(void);
/*!
 * @fn      GPIOE_Handler
 * @brief   Handles the interrupts, if the keypad is pressed it sets
 *          the Keypad_pressed variable.
*/
void GPIOE_Handler(void);

#endif // !KEYPAD_H_
