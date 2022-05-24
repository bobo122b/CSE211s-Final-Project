/*************************************************************************************************
 *!
 * @file 		lcd.c
 * @authors  	Abdelrahman Mostafa, Ahmed Shafeek, Ahmed Hamdy, Muhammed Emad, Abdelrahman Sameh 
 * @brief		contains the functions that defines how to use the LCD and connect it to Tiva
**************************************************************************************************/
#include "Microwave.h"

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
    SYSCTL_RCGCGPIO_R |= 0x02;					// unlock clock to port B
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {}
	GPIO_PORTB_LOCK_R  = GPIO_LOCK_KEY;			// unlock port B
    GPIO_PORTB_PCTL_R  = 0;						// define port B as gpio only
	GPIO_PORTB_CR_R   |= 0xFF;					// enable changes to port B
    GPIO_PORTB_DIR_R  |= 0xFF;					// enable port B as output
    GPIO_PORTB_DEN_R  |= 0xFF;					// enable port B as digital
    GPIO_PORTB_AMSEL_R = 0x00;					// disable analog function
    GPIO_PORTB_PUR_R   = 0;						// disable any PUR
	// initialize port A
	SYSCTL_RCGCGPIO_R |= 0x01;					// unlock clock to port A
	while ((SYSCTL_PRGPIO_R & 0x01) == 0) {}	
    GPIO_PORTA_PCTL_R  = 0;						// define port A as gpio only
	GPIO_PORTA_CR_R   |= 0x60;					// enable changes to pins PA5 and PA6
    GPIO_PORTA_DIR_R  |= 0x60;					// set PA5 and PA6 as output
    GPIO_PORTA_DEN_R  |= 0x60;					// set PA5 and PA6 as digital
    GPIO_PORTA_AMSEL_R = 0x00;					// disable analog function of port A
    GPIO_PORTA_PUR_R   = 0;						// disable any PUR

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
			
	LCD_cmd(clearDisplay);
	LCD_cmd(returnHome);	
}

void LCD_Write4bits(char data, char control)
{
	data 	&= 0xF0;					// check that data is only in the four MSB
	control &= 0xF0;					// check that only top bits are sent in control (RS)
	GPIO_PORTB_DATA_R  = data;			// set data before enabling the register
	GPIO_PORTA_DATA_R |= control;		// set control to make stable data before enabling
	GPIO_PORTA_DATA_R |= control | EN;	// enabling data
	SysTick_Wait1us(0);					// small delay
	GPIO_PORTA_DATA_R &= ~0x60;			// falling edge of enable pin to ensure writing data
	GPIO_PORTB_DATA_R = 0;				// removing data
}

void LCD_cmd(char command)
{
    LCD_Write4bits(command & 0xF0, 0); 						  // write MSB 4 bits of command
    LCD_Write4bits(command << 4, 0);   						  // write LSB 4 bits of control
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
        LCD_data(*(str+i));				// send string character by character
    }
}

void gotoxy(uint32_t x, uint32_t y)
{
	if (y==0)
		LCD_cmd(LCD_SETDDRAMADDR|(x+0x00));
	else if (y==1) 
		LCD_cmd(LCD_SETDDRAMADDR|(x+0x40));
}

void LCD_CountDown(void)
{
	// clearing the display to show the timer
	LCD_cmd(clearDisplay);
	LCD_cmd(returnHome);
		
	while ( (!(CookingIsDone) && state == COOKING))		// as long as state is cooking and cooking is not done yet 
	{
		
		if (state == PAUSED && UART5_DR_R =='p') {state =IDLE; break;}
		// if door became open (or is open from beginning) -> close door to continue
		if (!doorClosed || UART5_DR_R =='p') 
		{
			cookingTime[4]++;
			if (cookingTime[4]==':')
			{
				cookingTime[4]='0';
				cookingTime[3]++;
												
			}
			if (cookingTime[3]=='6' && cookingTime[4]=='0') {cookingTime[1]++; cookingTime[3]='0'; cookingTime[4]='0';}
			state = PAUSED; 
			break;
		}
		// door is closed -> start cooking
		gotoxy(5,0);
		LCD_WriteString(cookingTime);
		Bluetooth_SendString(cookingTime);
		Bluetooth_SendChar('\n');
		if (cookingTime[4]!='0')		// decrease 1  * seconds 
			cookingTime[4]--;
		else if (cookingTime[3]!='0')	// decrease 10 * seconds
		{
			cookingTime[3]--;
			cookingTime[4] = '9';
		}
		else if (cookingTime[1]!='0')	// decrease 1  * mins
		{
			cookingTime[1]--;
			cookingTime[3]='5';
			cookingTime[4]='9';
		}
		else if (cookingTime[0]!='0')	// decrease 10 * mins
		{
			cookingTime[0]--;
			cookingTime[1] = '9';
			cookingTime[3]= '5';
			cookingTime[4] = '9';
		}
									
		RotorStart();			// Start the motor and Countdown /*Systick delay is embedded in side the RotorStart function*/
		gotoxy(5,0);
		LCD_WriteString(cookingTime);	
		LCD_cmd(clearDisplay);
	}
}
