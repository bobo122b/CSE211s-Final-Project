#ifndef KEYPAD_H_
#define KEYPAD_H_

// Rows from C4:C7 
// Cols from E0:E3

/*!
 * @fn  Keypad_Init
 * @brief Initializes keypad pins with direction and digital enable
*/
void Keypad_Init(void);
char Keypad_getKey(void);
void GPIOE_Handler(void);

#endif // !KEYPAD_H_
