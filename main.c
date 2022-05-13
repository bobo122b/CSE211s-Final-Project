/*************************************************************************************************
 *!
 * @file 	main.c
 * @authors  Ahmed Shafeek, Ahmed Hamdy, Abdelrahman Mostafa, Muhammed Emad, Abdelrahman Sameh 
 * @brief	contains the main function to be applied on the Tiva 
**************************************************************************************************/
#include "Microwave.h"

void SystemInit(void) {
	PLL_Init();
	SysTick_Init();
	LCD_Init();
	Keypad_Init();
	Switches_Init();
}

int main()
{
	State state = IDLE;
	volatile char keypadInput;
	int Pause_Counter = -1; 
	
	//Cooking time variables
	char cookingTime[6]="00:00";
	float min;
	int seconds;
	
	char Weight; //Weight input in case of Beef of Chicken
	
	SystemInit();

	while (1)
	{
		switch (state)
		{
			case IDLE:
				GPIO_PORTF_DATA_R &=~(red|blue|green);
				LCD_cmd(clearDisplay);
				LCD_cmd(returnHome);
				LCD_WriteString("Input:A, B, C, D"); 
				LCD_cmd(SecondLine);
				LCD_WriteString("Input: ");
				keypadInput = Keypad_getKey();
				gotoxy(7,1);
				LCD_data(keypadInput);
				state = INPUT;
				break;
        }
    }
}
