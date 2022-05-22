/*************************************************************************************************
 *!
 * @file 		keypad.c
 * @authors  	Ahmed Hamdy, Ahmed Shafeek, Abdelrahman Mostafa, Muhammed Emad, Abdelrahman Sameh 
 * @brief		contains the keypad initialization and input functions
 *  
**************************************************************************************************/
#include "Microwave.h"

// the mapping of the Keypad
unsigned char map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void Keypad_Init(void) 
{
    SYSCTL_RCGCGPIO_R |= 0x14;              // Enable clock to ports C and E
    while((SYSCTL_PRGPIO_R & 0x14) == 0) {}
    GPIO_PORTC_LOCK_R = GPIO_LOCK_KEY;      // Unlocking port C
    GPIO_PORTC_CR_R  |= 0xF0;               // Enabling change to PC4~7
    GPIO_PORTE_CR_R  |= 0x0F;               // Enabling change to PE0~3
    GPIO_PORTE_DIR_R &= ~(0x0F);            // making port E pins as input 
    GPIO_PORTE_PDR_R |= 0x0F;               // enabling pull down resistor to port E
    GPIO_PORTC_DIR_R |= 0xF0;               // making port C pins as Output
    GPIO_PORTC_DEN_R |= 0xF0;               // enabling PC4~7 as digital pins
    GPIO_PORTE_DEN_R |= 0x0F;               // enabling PE0~3 as digital pins
}

char Keypad_getKey(void)
{
    const char col_select[] = {0x10, 0x20, 0x40, 0x80};     // defining columns' numbers in port C
    uint8_t col;
    unsigned char row = 0;
    unsigned char keyCode = 0, keyNum = 1;

    while(1)
    {
        col = 0;
        while(col < 4)
        {
            setColumn(col);                         // set only one column to loop on
            SysTick_Wait1us(2);                     // delay to make sure the column is set
            check_Row;                              // check all the rows at once
            if(row != 0) break;                     // break if row has any value (got an input from user)
            col++;
            if (Pause)
                return '~';                         // user clicked SW1 -> clear/return to IDLE
            else if (Start)     
                return 's';                         // user clicked SW2 -> cook
            if(col==4) col = 0;                     // loop while there's no input by user
        }

        // check which row was chosen, and return its value in keyCode
        check(keyCode, 0);

        setColumn(col);
        SysTick_Wait1us(2);
        check_Row;
        
        // making a new check to make sure the input was not a debouncing problem
        check(keyNum, 1);

        if(keyCode == keyNum) return keyCode;
    }

}

void GPIOE_Handler(void)
{
    //Check if E0 or E1 or E2 or E3 are high
    if (GPIO_PORTE_MIS_R & 0x1   || GPIO_PORTE_MIS_R & 0x2 || 
	    GPIO_PORTE_MIS_R & 0x4   ||	GPIO_PORTE_MIS_R & 0x8)
	{
		GPIO_PORTE_ICR_R |= 0xF;    // clear interrupt
		Keypad_pressed = 1;         // set to indicate that we've chosen input by keypad
	}
}
