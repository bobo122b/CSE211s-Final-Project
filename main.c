/*************************************************************************************************
 *!
 * @file 		main.c
 * @authors  	Ahmed Shafeek, Ahmed Hamdy, Abdelrahman Mostafa, Muhammed Emad, Abdelrahman Sameh 
 * @brief		contains the main function to be applied on the Tiva, the initialization. and
 * 				the applied states from the FSM diagram. 
**************************************************************************************************/
#include "Microwave.h"

void SystemInit(void) {
	PLL_Init();				// Initialize PLL to activate 80MHZ system clock
	SysTick_Init();			// Initialize SysTick to be used as timer
	LCD_Init();				// Initialize LCD pins
	Keypad_Init();			// Initialize Keypad pins
	Switches_Init();		// Initialize switches pins
	Buzzer_Init();			// Initialize buzzer pin
	Bluetooth_Init();		// Initialize Bluetooth pins
	StepperInit();          // Initialize Stepper Motor pins
}

// Declaration of extern variables in Microwave.h

char cookingTime[6];
State state;
volatile int Keypad_pressed;
volatile char Input;            // To store input from keypad or bluetooth
char Weight;                    // Weight input in case of Beef of Chicken

int main()
{
	char temp;
	int counter = 0; 			// Temp counter to be used for toggling the leds at the end of the COOKING state
	state = IDLE;
	char flag = 0;
	Keypad_pressed = 0;

	SystemInit();				// To initialize system pins

	while (1)
	{
		switch (state)
		{

//********************************************************************************************************
//**********************************************IDLE STATE************************************************		
//********************************************************************************************************

			case IDLE:
				IDLE_Pins;										// Make sure pins in IDLE state
				counter = 0;
				Weight = 'U';									// dummy initialization
				IDLE_Msg;
				Input = Bluetooth_ReceiveChar();                // Wait for a Bluetooth or a Keypad input from user
				if (Input == '~' || Input == 's')	            // if SW1 or SW2 is clicked -> back to idle state from beginning (same output defined in keypad and bluetooth)
					break;
				gotoxy(7,1);
				if (!(not_valid_input(Input)))
					LCD_data(Input);
				state = INPUT;
				break;
				
				
//********************************************************************************************************
//**********************************************INPUT STATE***********************************************			
//********************************************************************************************************				
			
			case INPUT:
				SysTick_Wait1ms(1000); 							// Wait before clearing the display
				LCD_cmd(clearDisplay);
				LCD_cmd(returnHome);
				if (Input == 'A')
				{
					Popcorn_Msg;								// since A is for popcorn, we display popcorn message on the LCD
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
						if (!doorClosed) Bluetooth_SendString("Door is open\n");
					    while(!doorClosed)
					    {
					        LCD_cmd(clearDisplay);
					        gotoxy(1,0);
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
					fill_array(cookingTime, "01:00");			// "01:00" for popcorn
				}
				else if (Input == 'B')
				{
					Beef_Msg;									// since B is for beef, we display beef message on the LCD
					Weight = Bluetooth_ReceiveChar();
					if (Weight == '~' || Weight == 'p')							// break back to IDLE state if user cancels the input
					{
						state = IDLE;
						break;
					}
													
					while (not_valid_num(Weight) || Weight=='0')
					{
						Err_msg;								// if the weight is invalid, we display an error message
						SysTick_Wait1ms(1000);
						LCD_cmd(clearDisplay);
						Beef_Msg;								// since B is for beef, we display beef message on the LCD
						Weight = Bluetooth_ReceiveChar();
						if (Weight == '~' || Weight == 'p')						// break back to IDLE state if user cancels the input
						{
							state = IDLE;
							break;
						}
					}
													
					LCD_data(Weight);
					SysTick_Wait1ms(1000);
					SW2_Msg;
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
					timing_string(Weight,Input);			// to calculate needed time for the specified weight, same function used with chicken
				} 
				else if (Input == 'C')
				{
					Chicken_Msg;								// since C is for chicken, we display chicken message on the LCD
					Weight = Bluetooth_ReceiveChar();
					if (Weight == '~' || Weight == 'p')							// break back to IDLE state if user cancels the input
					{
						state = IDLE;
						break;
					}
					while (not_valid_num(Weight) || Weight=='0')
					{
						Err_msg;								// if the weight is invalid, we display an error message
						SysTick_Wait1ms(1000);
						LCD_cmd(clearDisplay);
						Chicken_Msg;							// since C is for chicken, we display chicken message on the LCD
						//Weight = Keypad_getKey();				// We take the wight from the user
						Weight = Bluetooth_ReceiveChar();
						if (Weight == '~' || Weight == 'p')						// break back to IDLE state if user cancels the input
						{
							state = IDLE;
							break;
						}
					}
					LCD_data(Weight);
					SysTick_Wait1ms(1000);
					SW2_Msg;
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
				else if (Input == 'D')
				{
					Cooking_Time_Msg;							// since D is for cooking time, we display cooking time message on the LCD
					fill_array(cookingTime, "00:00");			// in the beginning, cookingTime will be equal to "00:00"
					gotoxy(5,0);
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
						Err_Cooking_Time;						// the cooking time input should be greater than 0 seconds
						state = INPUT;
						break;
					}
					while (not_valid_num(cookingTime[4]))
					{
						Err_msg;								// the user didn't input a valid number
						SysTick_Wait1ms(1000);
						LCD_cmd(clearDisplay);
						fill_array(cookingTime, "00:00");		// in the beginning, cookingTime will be equal to "00:00"
						gotoxy(5,0);
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
							Err_Cooking_Time;					// the cooking time input should be greater than 0 seconds
							break;
						}
					}
					LCD_cmd(clearDisplay);
					gotoxy(5,0);
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
							Err_Cooking_Time;					// if the cooking time is not valid (> 30:00 or < 00:01) go back to IDLE
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
						Err_msg;								// the user didn't input a valid number
						SysTick_Wait1ms(1000);
						state = IDLE;
						break;
					}
					LCD_cmd(clearDisplay);
					gotoxy(5,0);
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
							Err_Cooking_Time;					// if the cooking time is not valid go back to IDLE
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
						Err_msg;								// the user didn't input a valid number
						SysTick_Wait1ms(1000);
						state = IDLE;
						break;
					}
					LCD_cmd(clearDisplay);
					gotoxy(5,0);
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
							Err_Cooking_Time;					// if the cooking time is not valid go back to IDLE
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
						Err_msg;								// the user didn't input a valid number
						SysTick_Wait1ms(1000);
						state = IDLE;
						break;
					}
					LCD_cmd(clearDisplay);
					gotoxy(5,0);
					LCD_WriteString(cookingTime);				// display cookingTime on LCD
					Bluetooth_SendString(cookingTime);
					Bluetooth_SendChar('\n');
					SysTick_Wait1ms(1000);
					LCD_cmd(clearDisplay);
												
					if (not_valid_CookingTime(cookingTime))
					{
						Err_Cooking_Time;						// if the cooking time is not valid go back to IDLE
					}
					else
					{	
						SW2_Msg;								// if the cooking time is valid -> start cooking when SW2 if pressed
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
						        gotoxy(1,0);
						        LCD_WriteString("Close the Door");
						        SysTick_Wait1ms(300);
						    }
						    state = COOKING;
						}
					}								
				}
				else if (not_valid_input(Input))			// If the user doesn't input A, B, C, or D
				{
					Err_Invalid;
					SysTick_Wait1ms(2000);
					state = IDLE;								// go back to IDLE state
				}
				break;

//********************************************************************************************************
//**********************************************COOKING***************************************************			
//********************************************************************************************************								
				
			case COOKING:
				LCD_cmd(clearDisplay);
				LCD_cmd(returnHome);
				Light_On;										// white light on while cooking
				LCD_cmd(cursorOff);
				LCD_CountDown();								// start cooking timer
				if (CookingIsDone)								// cookingTime went down to "00:00"
				{
				    gotoxy(6,0);
					LCD_WriteString("DONE");
					Bluetooth_SendString("DONE\n");
					Light_Off;
					while (counter<5)
					{
						Blink_And_Buzz;							// blink the lights and buzz 3 times
						counter++;
					}
					LCD_cmd(cursorBlink);
					state = IDLE;								// go back to IDLE after cooking is finished
				}
							
				break;

//********************************************************************************************************
//**********************************************PAUSED STATE**********************************************		
//********************************************************************************************************				
				
			case PAUSED: 
				temp = UART5_DR_R;
				if (temp =='p') {state = IDLE; break;}   // break back to IDLE state if user decided to stop
				else if (temp =='s') {state = COOKING; break;} // break back to COOKING state if user resumes
				Paused_Msg;
				Blink;
				break;
		}
	}
}
