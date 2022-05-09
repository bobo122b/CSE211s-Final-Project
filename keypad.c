#include "tm4c123gh6pm.h"
#include "keypad.h"
#include "SysTick.h"

unsigned char map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Used to initialize Keypad first
void Keypad_Init(void) 
{
    SYSCTL_RCGCGPIO_R |= 0x14;  // Enable clock to ports C and E
    while((SYSCTL_PRGPIO_R & 0x14) == 0) {}
		GPIO_PORTC_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTC_CR_R  |= 0xF0;      // Enabling change to PC4~7
    GPIO_PORTE_CR_R  |= 0x0F;      // Enabling change to PE1~4
    GPIO_PORTE_DIR_R &= ~(0x0F);   // making port E pins as input
    GPIO_PORTE_PDR_R |= 0x0F;      // enabling pull down resistor to port E
		GPIO_PORTC_DIR_R |= 0xF0;
    GPIO_PORTC_DEN_R |= 0xF0;      // enabling PC4~7 as digital pins
    GPIO_PORTE_DEN_R |= 0x0F;      // enabling PE1~4 as digital pins
}

char Keypad_getKey(void)
{
    int i, j;
    while(1)
    {
        for (i = 0; i < 4; i++)     // Scan columns
        {
            GPIO_PORTC_DATA_R = (1U << (i+4));    // set columns to high
            if (!(NVIC_ST_CTRL_R & 0x05))
                SysTick_Init();
            SysTick_Wait1us(2);         // delay between scans
            for (j = 0; j < 4; j++)     // Scan rows
            {
                if ((GPIO_PORTE_DATA_R & 0x0F) & (1U << (j)))
                    return map[j][i];
            }
        }
    }
}
