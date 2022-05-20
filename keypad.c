#include "Microwave.h"

unsigned char map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void Keypad_Init(void) 
{
    SYSCTL_RCGCGPIO_R |= 0x14;  // Enable clock to ports C and E
    while((SYSCTL_PRGPIO_R & 0x14) == 0) {}
    GPIO_PORTC_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTC_CR_R  |= 0xF0;      // Enabling change to PC4~7
    GPIO_PORTE_CR_R  |= 0x0F;      // Enabling change to PE0~3
    GPIO_PORTE_DIR_R &= ~(0x0F);   // making port E pins as input 
    GPIO_PORTE_PDR_R |= 0x0F;      // enabling pull down resistor to port E
    GPIO_PORTC_DIR_R |= 0xF0;      // making port C pins as Output
    GPIO_PORTC_DEN_R |= 0xF0;      // enabling PC4~7 as digital pins
    GPIO_PORTE_DEN_R |= 0x0F;      // enabling PE0~3 as digital pins
}

char Keypad_getKey(void)
{
    const char col_select[] = {0x10, 0x20, 0x40, 0x80};
    uint8_t col;
    unsigned char row = 0;
    unsigned char keyCode = 0, keyNum = 1;

    while(1)
    {
        col = 0;
        while(col < 4)
        {
            GPIO_PORTC_DATA_R &= ~0xF0;
            GPIO_PORTC_DATA_R |= col_select[col];
            SysTick_Wait1us(10);
            row = GPIO_PORTE_DATA_R & 0x0F;
            if(row != 0) break;
            col++;
            if (Pause)
                return '~';
            else if (Start)
                return 's';
            if(col==4) col = 0;
        }

        if(row == 0x01) keyCode = map[0][col];
        else if(row == 0x02) keyCode = map[1][col];
        else if(row == 0x04) keyCode = map[2][col];
        else if(row == 0x08) keyCode = map[3][col];
        else keyCode = 0;

        GPIO_PORTC_DATA_R &= ~0xF0;
        GPIO_PORTC_DATA_R |= col_select[col];
        SysTick_Wait1us(10);
        row = GPIO_PORTE_DATA_R & 0x0F;

        if(row == 0x01) keyNum = map[0][col];
        else if(row == 0x02) keyNum = map[1][col];
        else if(row == 0x04) keyNum = map[2][col];
        else if(row == 0x08) keyNum = map[3][col];
        else keyNum = 1;

        if(keyCode == keyNum) return keyCode;
    }

}

void GPIOE_Handler(void)
{
		 if (GPIO_PORTE_MIS_R& 0x1 || GPIO_PORTE_MIS_R& 0x2 
					||GPIO_PORTE_MIS_R& 0x4 ||	GPIO_PORTE_MIS_R& 0x8) //Check if E0 or E1 or E2 or E3 are high
				{
					GPIO_PORTE_ICR_R |= 0xF;
					Keypad_pressed = 1;
				}
}
