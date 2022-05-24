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
	Bluetooth_Init();		  // Initialize Bluetooth pins
	StepperInit();     // Initialize Stepper Motor pins
}


//Declaration of extern variables in Microwave.h
char cookingTime[6];
State state;

int main()
{
		state = IDLE;
		//Cooking time variables
		fill_array(cookingTime,"00:00");
		volatile char Input; //to store input from key pad
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
								Input = Bluetooth_ReceiveChar(); //while(waiting for blutooth) if (pause || Start) nop else if (bluetooth input){input,break} else if (keypad input){input,break} 
								if (Input == '~' || Input == 's')	// if SW1 or SW2 is clicked -> back to idle state from beginning (same output defined in keypad and bluetooth)
									break;
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
													while (!Start && ((UART5_FR_R & (1<<4)) != 0)) {
													if (Pause) {state = IDLE; flag = 1; break;}		// break back to IDLE state if user cancels the input
													} //to prevent breaking case
													temp = UART5_DR_R;
													if (temp == 'p')
													{
														state = IDLE;
														flag  = 1;
													}
													else if (temp == 's')
													{
															if (!doorClosed) Bluetooth_SendString("Door is open");
														while(!doorClosed)
														{
															LCD_cmd(clearDisplay);
															LCD_WriteString("Close the Door");
															SysTick_Wait1ms(300);
														}
														state = COOKING;
													}
													if (flag == 1)
													{
														flag = 0;
														break;
													}
													fill_array(cookingTime, "01:00");
												}
							else if (keypadInput == 'B')
												{
													LCD_WriteString("Beef Weight?");
													LCD_cmd(SecondLine);
													LCD_cmd(cursorBlink);
													Weight = Keypad_getKey();	
													Weight = Bluetooth_ReceiveChar();
													if (Weight == '~' || Weight == 'p')							// break back to IDLE state if user cancels the input
													{
														state = IDLE;
														break;
													}
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
														Weight = Bluetooth_ReceiveChar();
														if (Weight == '~' || Weight == 'p')						// break back to IDLE state if user cancels the input
														{
															state = IDLE;
															break;
														}
																					}
													
													LCD_data(Weight);
													SysTick_Wait1ms(2000);
													LCD_cmd(clearDisplay);
													LCD_cmd(cursorOff);
													LCD_WriteString("SW2 to start");
													while (!Start && ((UART5_FR_R & (1<<4)) != 0)) {
													if (Pause) {state = IDLE; flag = 1; break;}		// break back to IDLE state if user cancels the input
													} //to prevent breaking case
													temp = UART5_DR_R;
													if (temp == 'p')
													{
														state = IDLE;
														flag  = 1;
													}
													else if (temp == 's')
													{
															if (!doorClosed) Bluetooth_SendString("Door is open");
														while(!doorClosed)
														{
															LCD_cmd(clearDisplay);
															LCD_WriteString("Close the Door");
															SysTick_Wait1ms(300);
														}
														state = COOKING;
													}
													if (flag == 1)
													{
														flag = 0;
														break;
													}
													timing_string(Weight,Input);
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
			                	else if (Input == 'D')
												{
														// since D is for cooking time, we display cooking time message on the LCD
														LCD_WriteString("Cooking time?");
														LCD_cmd(cursorOff);
														SysTick_Wait1ms(1000);
														LCD_cmd(clearDisplay);
														LCD_cmd(cursorBlink);
														Bluetooth_SendString("Cooking time?\n");
														Bluetooth_SendString("Enter p to cancel\n");
														fill_array(cookingTime, "00:00");			// in the beginning, cookingTime will be equal to "00:00"
														LCD_WriteString(cookingTime);				// display cookingTime on LCD
														Bluetooth_SendString(cookingTime);
														Bluetooth_SendChar('\n');

														// take the first number from user		
														//cookingTime[4]= Keypad_getKey();			// take input from user in the last cooking time index
														cookingTime[4] = Bluetooth_ReceiveChar();
														if (cookingTime[4] == '~' || cookingTime[4] == 'p')					// clear the input and go back to IDLE
														{
															state = IDLE;
															break;
														}
														else if (cookingTime[4] == 's')
														{
															// the cooking time input should be greater than 0 seconds
															LCD_cmd(clearDisplay);
															LCD_cmd(cursorOff);
															LCD_WriteString("Err");
															LCD_cmd(SecondLine);
															LCD_WriteString("00:01 to 30:00");
															Bluetooth_SendString("Err\n00:01 to 30:00\n");
															state = INPUT;
															break;
														}
														while (not_valid_num(cookingTime[4]))
														{
															// the user didn't input a valid number
															LCD_cmd(clearDisplay);
															LCD_WriteString("Err");
															LCD_cmd(SecondLine);
															LCD_WriteString("Enter 1 to 9\0");
															Bluetooth_SendString("Err\nEnter 1 to 9\n");
								
															SysTick_Wait1ms(1000);
															LCD_cmd(clearDisplay);
															fill_array(cookingTime, "00:00");		// in the beginning, cookingTime will be equal to "00:00"
															LCD_WriteString(cookingTime);			// display cookingTime on LCD
															Bluetooth_SendString(cookingTime);
															Bluetooth_SendChar('\n');

															//cookingTime[4]= Keypad_getKey();
															cookingTime[4] = Bluetooth_ReceiveChar();
															if (cookingTime[4] == '~' || cookingTime[4] == 'p')				// Clear the input and go back to IDLE
															{
																state = IDLE;
																break;
															}
															else if (cookingTime[4] == 's')
															{
																// the cooking time input should be greater than 0 seconds
																LCD_cmd(clearDisplay);
																LCD_cmd(cursorOff);
																LCD_WriteString("Err");
																LCD_cmd(SecondLine);
																LCD_WriteString("00:01 to 30:00");
																Bluetooth_SendString("Err\n00:01 to 30:00\n");
																break;
															}
														}
														LCD_cmd(clearDisplay);
														LCD_WriteString(cookingTime);				// display cookingTime on LCD
														Bluetooth_SendString(cookingTime);
														Bluetooth_SendChar('\n');
														SysTick_Wait1ms(500);

														// take the second number from user			
														rearrange(cookingTime);						// rearrange cookingTime to be ready for next input
														//cookingTime[4]= Keypad_getKey();			// take input from user in the last cooking time index
														cookingTime[4] = Bluetooth_ReceiveChar();
														if (cookingTime[4] == '~' || cookingTime[4] == 'p')					// Clear the input and go back to IDLE
														{
															state = IDLE;
															break;
														}
														else if (cookingTime[4] == 's')
														{

															dearrange(cookingTime);					// dearrange cookingTime and make it ready for evaluation						
															cookingTime[3] = '0';					// to make sure that the dearrange happened correctly
															if (not_valid_CookingTime(cookingTime))
															{
																// if the cooking time is not valid (> 30:00 or < 00:01) go back to IDLE
																LCD_cmd(clearDisplay);
																LCD_cmd(cursorOff);
																LCD_WriteString("Err");
																LCD_cmd(SecondLine);
																LCD_WriteString("00:01 to 30:00");
																Bluetooth_SendString("Err\n00:01 to 30:00\n");
																SysTick_Wait1ms(1000);
																state = IDLE;
																break;
															}
															else
															{
																state = COOKING;					// if the cooking time is valid -> start cooking
																break;
															}
														}
														if (not_valid_num(cookingTime[4]))
														{
															// the user didn't input a valid number
															LCD_cmd(clearDisplay);
															LCD_WriteString("Err");
															LCD_cmd(SecondLine);
															LCD_WriteString("Enter 1 to 9\0");
															Bluetooth_SendString("Err\nEnter 1 to 9\n");
															SysTick_Wait1ms(1000);
															state = IDLE;
															break;
														}
														LCD_cmd(clearDisplay);
														LCD_WriteString(cookingTime);				// display cookingTime on LCD
														Bluetooth_SendString(cookingTime);
														Bluetooth_SendChar('\n');
														SysTick_Wait1ms(500);
																						
														rearrange(cookingTime);						// rearrange cookingTime to be ready for next input
														//cookingTime[4]= Keypad_getKey();			// take input from user in the last cooking time index
														cookingTime[4] = Bluetooth_ReceiveChar();
														if (cookingTime[4] == '~' || cookingTime[4] == 'p')					// Clear the input and go back to IDLE
															break;
														else if (cookingTime[4] == 's')
														{
															dearrange(cookingTime);					// dearrange cookingTime and make it ready for evaluation
															cookingTime[1] = '0';					// to make sure that the dearrange happened correctly
															if (not_valid_CookingTime(cookingTime))
															{
																LCD_cmd(clearDisplay);
																// if the cooking time is not valid go back to IDLE
																LCD_cmd(clearDisplay);
																LCD_cmd(cursorOff);
																LCD_WriteString("Err");
																LCD_cmd(SecondLine);
																LCD_WriteString("00:01 to 30:00");
																Bluetooth_SendString("Err\n00:01 to 30:00\n");
																SysTick_Wait1ms(1000);
																state = IDLE;
																break;
															}
															else {
																state = COOKING;					// if the cooking time is valid -> start cooking
																break;
															}
														}
														if (not_valid_num(cookingTime[4]))
														{
															// the user didn't input a valid number
															LCD_cmd(clearDisplay);
															LCD_WriteString("Err");
															LCD_cmd(SecondLine);
															LCD_WriteString("Enter 1 to 9\0");
															Bluetooth_SendString("Err\nEnter 1 to 9\n");
															SysTick_Wait1ms(1000);
															state = IDLE;
															break;
														}
														LCD_cmd(clearDisplay);
														LCD_WriteString(cookingTime);				// display cookingTime on LCD
														Bluetooth_SendString(cookingTime);
														Bluetooth_SendChar('\n');
														SysTick_Wait1ms(500);
																					
														rearrange(cookingTime);						// rearrange cookingTime to be ready for next input
														//cookingTime[4]= Keypad_getKey();			// take input from user in the last cooking time index
														cookingTime[4] = Bluetooth_ReceiveChar();
														if (cookingTime[4] == '~' || cookingTime[4] == 'p')					// Clear the input and go back to IDLE
															break;
														else if (cookingTime[4] == 's')
														{
															dearrange(cookingTime);					// dearrange cookingTime and make it ready for evaluation
															cookingTime[0] = '0';					// to make sure that the dearrange happened correctly
															if (not_valid_CookingTime(cookingTime))
															{
																LCD_cmd(clearDisplay);
																// if the cooking time is not valid go back to IDLE
																LCD_cmd(clearDisplay);
																LCD_cmd(cursorOff);
																LCD_WriteString("Err");
																LCD_cmd(SecondLine);
																LCD_WriteString("00:01 to 30:00");
																Bluetooth_SendString("Err\n00:01 to 30:00\n");					
																SysTick_Wait1ms(1000);
																state = IDLE;
																break;
															}
															else
															{ 
																state = COOKING;					// if the cooking time is valid -> start cooking
																break;
															}
														}
														if (not_valid_num(cookingTime[4]))
														{
																						// the user didn't input a valid number
															LCD_cmd(clearDisplay);
															LCD_WriteString("Err");
															LCD_cmd(SecondLine);
															LCD_WriteString("Enter 1 to 9\0");
															Bluetooth_SendString("Err\nEnter 1 to 9\n");
															SysTick_Wait1ms(1000);
															state = IDLE;
															break;
														}
														LCD_cmd(clearDisplay);
														LCD_WriteString(cookingTime);				// display cookingTime on LCD
														Bluetooth_SendString(cookingTime);
														Bluetooth_SendChar('\n');
														SysTick_Wait1ms(1000);
														LCD_cmd(clearDisplay);
																					
														if (not_valid_CookingTime(cookingTime))
														{
															// if the cooking time is not valid go back to IDLE
															LCD_cmd(clearDisplay);
															LCD_cmd(cursorOff);
															LCD_WriteString("Err");
															LCD_cmd(SecondLine);
															LCD_WriteString("00:01 to 30:00");
															Bluetooth_SendString("Err\n00:01 to 30:00\n");
														}
														else
														{	
															LCD_cmd(clearDisplay);
															LCD_cmd(cursorOff);
															LCD_WriteString("SW2 to start");/* we tell the user that he needs to press on SW2 to start cooking */\
															Bluetooth_SendString("Enter s to start, p to cancel\n");					// if the cooking time is valid -> start cooking when SW2 if pressed
															while (!Start && ((UART5_FR_R & (1<<4)) != 0)) {
																if (Pause) {state = IDLE; break;}	// break back to IDLE state if user cancels the input
															} //to prevent breaking case
															temp = UART5_DR_R;
															if (temp == 'p')
															{
																state = IDLE;
																break;
															}
															else if (temp == 's')
															{
																	if (!doorClosed) Bluetooth_SendString("Door is open");
																while(!doorClosed)
																{
																	LCD_cmd(clearDisplay);
																	LCD_WriteString("Close the Door");
																	SysTick_Wait1ms(300);
																}
																state = COOKING;
															}
														}
				else if (not_valid_input(Input))
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
							Bluetooth_SendString("DONE\n");
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
					temp = UART5_DR_R;
					if (temp =='p') {state = IDLE; break;}   // break back to IDLE state if user decided to stop
					else if (temp =='s') {state = COOKING; break;} // break back to COOKING state if user resumes
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
