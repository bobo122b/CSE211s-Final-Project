#include "lcd.h"

/* 
    VSS -> gnd
    VDD -> 5V
    V0  -> Pot
    Rs  -> PA5
    R/W -> gnd
    E   -> PA6
    DB0 -> not connected
    DB1 -> not connected
    DB2 -> not connected
    DB3 -> not connected
    DB4 -> PB4
    DB5 -> PB5
    DB6 -> PB6
    DB7 -> PB7
    A   -> 5V
    K   -> gnd
*/

// we set RS to 1 to write data to DDRAM
// SysTick must be initialized first

void LCD_Init(void)
{
    // Initalize Port B as digital output
    SYSCTL_RCGCGPIO_R |= 0x02;
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {}
		GPIO_PORTB_LOCK_R = 0x4C4F434B;
    GPIO_PORTB_PCTL_R = 0;
		GPIO_PORTB_CR_R  |= 0xFF;
    GPIO_PORTB_DIR_R |= 0xFF;
    GPIO_PORTB_DEN_R |= 0xFF;
    GPIO_PORTB_AMSEL_R = 0x00;
    GPIO_PORTB_PUR_R = 0;
		// initialize port A
		SYSCTL_RCGCGPIO_R |= 0x01;
		while ((SYSCTL_PRGPIO_R & 0x01) == 0) {}
    GPIO_PORTA_PCTL_R = 0;
		GPIO_PORTA_CR_R  |= 0x60;
    GPIO_PORTA_DIR_R |= 0x60;
    GPIO_PORTA_DEN_R |= 0x60;
    GPIO_PORTA_AMSEL_R = 0x00;
    GPIO_PORTA_PUR_R = 0;
    // LCD initialization from datasheet
    SysTick_Wait1ms(15);
    LCD_cmd(0x30);
    SysTick_Wait1ms(5);
    LCD_cmd(0x30);
    SysTick_Wait1us(100);
    LCD_cmd(0x30);
    LCD_cmd(0x20);
    LCD_cmd(setTo4Bits);
    LCD_cmd(entryMode);
		LCD_cmd(cursorBlink);
    SysTick_Wait1ms(2);
}

void LCD_Write4bits(char data, char control)
{/*
    data &= 0xF0;   // check that data is only in the four MSB
    control &= 0xF0; //check that only top bits are sent in control (RS)
    GPIO_PORTB_DATA_R = data | control;
    GPIO_PORTB_DATA_R = data | control | EN;
    SysTick_Wait1us(1);
    GPIO_PORTB_DATA_R = data;
    GPIO_PORTB_DATA_R = 0;*/
	data &= 0xF0;
	control &= 0xF0;
	GPIO_PORTB_DATA_R = data;
	GPIO_PORTA_DATA_R |= control;
	GPIO_PORTA_DATA_R |= control | EN;
	SysTick_Wait1us(0);
	GPIO_PORTA_DATA_R &= ~0x60;
	GPIO_PORTB_DATA_R = 0;
}

void LCD_cmd(char command)
{
    LCD_Write4bits(command & 0xF0, 0); // write MSB 4 bits of command
    LCD_Write4bits(command << 4, 0);   // write LSB 4 bits of control
    if ((command == clearDisplay) || (command == returnHome)) // they take longer
        SysTick_Wait1ms(2);
    else
        SysTick_Wait1us(40);
}

// for sending characters
void LCD_data(char data)
{
    LCD_Write4bits(data & 0xF0, RS);    // send MSB 4 bits, and set RS
    LCD_Write4bits(data << 4, RS);      // send LSB 4 bits, and set RS
    SysTick_Wait1us(40);
}

void LCD_WriteString(char* str)
{
    uint32_t i;
    for (i = 0; (*(str+i) != '\0'); i++)
    {
        LCD_data(*(str+i));
    }
}

void gotoxy(uint32_t x, uint32_t y)
{
	if (y==0)
		LCD_cmd(LCD_SETDDRAMADDR|(x+0x00));
	else if (y==1) 
			LCD_cmd(LCD_SETDDRAMADDR|(x+0x40));
}

