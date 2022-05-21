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
	Buzzer_Init();
}

//Declaration of extern variables in Microwave.h
char cookingTime[6];
State state;

int main()
{
		state = IDLE;
		//Cooking time variables
		fill_array(cookingTime,"00:00");
		volatile char keypadInput; //to store input from key pad
		char Weight; //Weight input in case of Beef of Chicken
		int counter = 0; //temp counter to be used for toggling the leds at the end of the COOKING state
    	
	  SystemInit();

	while (1)
	{
		switch (state)
		{

//********************************************************************************************************
//**********************************************IDLE STATE************************************************		
//********************************************************************************************************

			case IDLE:
								GPIO_PORTF_DATA_R &=~(red|blue|green);
								GPIO_PORTA_DATA_R &=~buzzer;
								GPIO_PORTF_ICR_R |= SW2|SW1;  // Clear Interrupt
								GPIO_PORTA_ICR_R |= SW3;  // Clear Interrupt
								counter = 0;
								Weight = 'U'; 
								LCD_cmd(clearDisplay);
								LCD_cmd(returnHome);
								LCD_WriteString("Enter A,B,C,or D"); 
								LCD_cmd(SecondLine);
								LCD_WriteString("Input: ");		
								keypadInput = Keypad_getKey();
								gotoxy(7,1);
								LCD_data(keypadInput);
								state = INPUT;
								break;
				
				
//********************************************************************************************************
//**********************************************INPUT STATE***********************************************			
//********************************************************************************************************				
			
				case INPUT:
							SysTick_Wait1ms(2000); //Wait 2 seconds before clearing the display
							LCD_cmd(clearDisplay);
							LCD_cmd(returnHome);
							if (keypadInput == 'A')
												{
													LCD_cmd(cursorOff);
													LCD_WriteString("Popcorn");
													SysTick_Wait1ms(2000);
													LCD_cmd(clearDisplay);
													LCD_WriteString("SW2 to start");
													while (!Start); //to prevent breaking case
													fill_array(cookingTime,"01:00");
												}
							else if (keypadInput == 'B')
												{
													LCD_WriteString("Beef Weight?");
													LCD_cmd(SecondLine);
													LCD_cmd(cursorBlink);
													Weight = Keypad_getKey();	
													
													while (not_valid_num(Weight) || Weight=='0')
													{
														LCD_cmd(clearDisplay);
														LCD_WriteString("Err");
														LCD_cmd(SecondLine);
														LCD_WriteString("Enter 1 to 9");
														SysTick_Wait1ms(2000);
														LCD_cmd(clearDisplay);
														LCD_WriteString("Beef Weight?");
														LCD_cmd(cursorBlink);
														Weight = Keypad_getKey();
													}
													
													LCD_data(Weight);
													SysTick_Wait1ms(2000);
													LCD_cmd(clearDisplay);
													LCD_cmd(cursorOff);
													LCD_WriteString("SW2 to start");
													while (!Start); //to prevent breaking case
													timing_string(Weight,keypadInput);
												} 
						 else if (keypadInput == 'C')
												{
													LCD_WriteString("Chicken Weight?");
													LCD_cmd(SecondLine);
													LCD_cmd(cursorBlink);
													Weight = Keypad_getKey();
													while (not_valid_num(Weight) || Weight=='0')
													{
														LCD_cmd(clearDisplay);
														LCD_WriteString("Err");
														LCD_cmd(SecondLine);
														LCD_WriteString("Enter 1 to 9");
														SysTick_Wait1ms(2000);
														LCD_cmd(clearDisplay);
														LCD_WriteString("Chicken Weight?");
														Weight = Keypad_getKey();
													}
													LCD_data(Weight);
													SysTick_Wait1ms(2000);
													LCD_cmd(clearDisplay);
													LCD_cmd(cursorOff);
													LCD_WriteString("SW2 to start");
													while (!Start); //to prevent breaking case
													timing_string(Weight,keypadInput);
												}
						 else if (keypadInput == 'D')
												{
													LCD_WriteString("Cooking time?");
													LCD_cmd(cursorOff);
													SysTick_Wait1ms(2000);
													LCD_cmd(clearDisplay);
													LCD_cmd(cursorBlink);
													
													cookingTime[0]= Keypad_getKey(); 
													LCD_WriteString("00:0");
													LCD_data(cookingTime[0]);
													SysTick_Wait1ms(500);
													
													
													cookingTime[1]= Keypad_getKey();
													LCD_cmd(clearDisplay);
													LCD_WriteString("00:");
													LCD_data(cookingTime[0]);
													LCD_data(cookingTime[1]);
													cookingTime[2]= ':';
													SysTick_Wait1ms(500);
													
													cookingTime[3]= Keypad_getKey();
													LCD_cmd(clearDisplay);
													LCD_data('0');
													LCD_data(cookingTime[0]);
													LCD_data(cookingTime[2]);
													LCD_data(cookingTime[1]);
													LCD_data(cookingTime[3]);
													SysTick_Wait1ms(500);
													
													cookingTime[4]= Keypad_getKey();
													LCD_cmd(clearDisplay);
													cookingTime[5]=0;
													LCD_WriteString(cookingTime);
													SysTick_Wait1ms(2000);
													LCD_cmd(clearDisplay);
												
													if (not_valid_CookingTime(cookingTime))
													{
															LCD_cmd(cursorOff);
															LCD_WriteString("Err");
															LCD_cmd(SecondLine);
															LCD_WriteString("01:00 to 30:00");
													}
													else
													{	
														LCD_cmd(cursorOff);
														LCD_WriteString("SW2 to start");
														while (!Start); //to prevent breaking case
													}								
											}
				else if (not_valid_input(keypadInput))
								{
											LCD_cmd(cursorOff);
											LCD_WriteString("Err");
											LCD_cmd(SecondLine);
											LCD_WriteString("Invalid input");
											SysTick_Wait1ms(2000);
											state = IDLE;
								}
								break;

//********************************************************************************************************
//**********************************************COOKING***************************************************			
//********************************************************************************************************								
				
				case COOKING:
						LCD_cmd(clearDisplay);
						LCD_cmd(returnHome);
						GPIO_PORTF_DATA_R |=(red|green|blue);
						LCD_cmd(cursorOff);
						LCD_CountDown();
						if (CookingIsDone)
						{
							LCD_WriteString("DONE");
							while (counter<5)
							{
									GPIO_PORTF_DATA_R ^= (red|green|blue);
									GPIO_PORTA_DATA_R ^=buzzer;
									SysTick_Wait1ms(1000);
									counter++;
							}
							LCD_cmd(cursorBlink);
							state = IDLE;
						}
							
					break;

//********************************************************************************************************
//**********************************************PAUSED STATE**********************************************		
//********************************************************************************************************				
				
				case PAUSED: 
					LCD_cmd(clearDisplay);
					LCD_cmd(cursorOff);
					LCD_WriteString(cookingTime);
					LCD_cmd(SecondLine);
					LCD_WriteString("PAUSED");
					GPIO_PORTF_DATA_R ^=(red|green|blue);
					SysTick_Wait1ms(500);
				break;
		}
	}
}
