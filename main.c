/*************************************************************************************************
 *!
 * @file 	main.c
 * @author  Ahmed Shafeek, Ahmed Hamdy, Abdelrahman Mostafa, Muhammed Emad, Abdelrahman Sameh 
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
				LCD_WriteString("ttf7 eh??"); 
				LCD_cmd(SecondLine);
				LCD_WriteString("Input: ");
				keypadInput = Keypad_getKey();
				gotoxy(7,1);
				LCD_data(keypadInput);
				state = INPUT;
				break;
			
			case INPUT:
				SysTick_Wait1ms(2000); //Wait 2 seconds before clearing the display
				LCD_cmd(clearDisplay);
				LCD_cmd(returnHome);
				if (keypadInput == 'A')
				{
					LCD_WriteString("Popcorn");
					while (!(doorClosed));
					state = COOKING;
					fill_array(cookingTime,"00:59");
					LCD_CountDown(cookingTime);
					LCD_cmd(clearDisplay);
				}
				else if (keypadInput == 'B')
				{
					LCD_WriteString("Beef Weight?");
					Weight = Keypad_getKey();	

					while (not_valid_num(Weight))
					{
						LCD_cmd(clearDisplay);
						LCD_WriteString("Err");
						SysTick_Wait1ms(2000);
						LCD_cmd(clearDisplay);
						LCD_WriteString("Beef Weight?");
						Weight = Keypad_getKey();
					}
					LCD_cmd(SecondLine);
					LCD_data(Weight);
					SysTick_Wait1ms(2000);
					LCD_cmd(clearDisplay);
										
					while(!(doorClosed));
					state = COOKING;
					min = (Weight-'0') * 0.5;
					seconds = (int)(min * 60) %60;
					min = (int)(min);
					cookingTime[4] = '0' ;
					cookingTime[3]=(seconds)/10 +'0';
					cookingTime[2]=':';
					cookingTime[1]=min+'0';
					cookingTime[0]='0';
					LCD_CountDown(cookingTime);
				} 
				else if (keypadInput == 'C')
				{
					LCD_WriteString("Chicken Weight?");
					Weight = Keypad_getKey();
					while (not_valid_num(Weight))
					{
						LCD_cmd(clearDisplay);
						LCD_WriteString("Err");
						SysTick_Wait1ms(2000);
						Weight = Keypad_getKey();
					}
					LCD_cmd(clearDisplay);
					LCD_data(Weight);
											
					while(!(doorClosed));
					state = COOKING;
					min = (Weight-'0') * 0.2;
					seconds = (int)(min * 60) %60;
					min = (int)(min);
					cookingTime[4] = '0' ;
					cookingTime[3]=(seconds)/10 +'0';
					cookingTime[2]=':';
					cookingTime[1]=min+'0';
					cookingTime[0]='0'; 
				}
				else if (keypadInput == 'D')
				{
					LCD_WriteString("Cooking time?");
					SysTick_Wait1ms(2000);
					LCD_cmd(clearDisplay);
					cookingTime[0]= Keypad_getKey(); 
					LCD_WriteString("00:0");
					gotoxy(0,4);
					LCD_data(cookingTime[0]);
					cookingTime[1]= Keypad_getKey();
					LCD_cmd(clearDisplay);
					LCD_WriteString("00:");
					gotoxy(0,3);
					LCD_data(cookingTime[0]);
					LCD_data(cookingTime[1]);
					cookingTime[2]= ':';
					cookingTime[3]= Keypad_getKey();
					LCD_cmd(clearDisplay);
				    LCD_WriteString("0");
					gotoxy(0,1);
					LCD_data(cookingTime[0]);
					LCD_data(cookingTime[1]);
					LCD_data(cookingTime[2]);
					LCD_data(cookingTime[3]);
					cookingTime[4]= Keypad_getKey();
					LCD_cmd(clearDisplay);
					cookingTime[5]=0;
					LCD_WriteString(cookingTime);
				}
				else 
				{
					LCD_WriteString("ZOBR");
					SysTick_Wait1ms(2000);
					state = IDLE;
				}
				break;
							
			case COOKING:
				GPIO_PORTF_DATA_R |= red|blue|green;
				break;
					
			case PAUSED:
				Pause_Counter++;
				if (Pause_Counter==2)
				{
					LCD_WriteString("00:00");
					state=IDLE;
				}
				break;
		}
	}
}
