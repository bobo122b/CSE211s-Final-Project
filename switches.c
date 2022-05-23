/*************************************************************************************************
 *!
 * @file 		switches.c
 * @authors  	Ahmed Shafeek, Abdelrahman Mostafa, Ahmed Hamdy, Muhammed Emad, Abdelrahman Sameh 
 * @brief		contains the functions that defines the behaviour of the switches.
**************************************************************************************************/
#include "Microwave.h"

void Switches_Init(void)
{
		// Initializing port F
		SYSCTL_RCGCGPIO_R |=0x20;
		while ((SYSCTL_PRGPIO_R&0x20)==0){}
		GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
		GPIO_PORTF_CR_R = red|blue|green|SW1|SW2;
		GPIO_PORTF_DEN_R |= red|blue|green|SW1|SW2;
		GPIO_PORTF_DIR_R |= red|green|blue;
		GPIO_PORTF_DIR_R  &= (~SW1)|(~SW2);
		GPIO_PORTF_PUR_R |= SW1|SW2; 				//SW1, SW2 input
		GPIO_PORTF_AMSEL_R = 0;
		GPIO_PORTF_AFSEL_R = 0;
		GPIO_PORTF_PCTL_R = 0;
			
		// Initializin port A
		SYSCTL_RCGCGPIO_R |=0x01;
		while ((SYSCTL_PRGPIO_R&0x01)==0){}
		GPIO_PORTA_LOCK_R = GPIO_LOCK_KEY;
		GPIO_PORTA_CR_R |= IR;	
		GPIO_PORTA_DEN_R |= IR;
		GPIO_PORTA_DIR_R &= ~(IR); 				//IR input
		GPIO_PORTA_PUR_R |= IR;
		GPIO_PORTA_AMSEL_R = 0;
		GPIO_PORTA_AFSEL_R = 0;
		GPIO_PORTA_PCTL_R = 0;
	
		// Interrupt initialization for port F
		NVIC_EN0_R       |= 1U<<30; 				  //Enabling PORTF Interrupt 
		GPIO_PORTF_IS_R  &= (~SW1)|(~SW2); 		     	  //Interrupt sense   0 for edge && 1 for level		
		GPIO_PORTF_IBE_R &= (~SW1)|(~SW2);  		          //Interrupt both edges 0 Edge controlled by GPIOIEV && 1 Both edges
		GPIO_PORTF_IEV_R &= (~SW1)|(~SW2); 	 		  //Interrupt Event 0 Falling && 1 Rising
		GPIO_PORTF_ICR_R |= SW1|SW2;   				  //Clear any prior interrupt
		GPIO_PORTF_IM_R  |= SW1|SW2;  		       	          //Interrupt Mask  1 Enable Interrupt &&  0 Disable interrupt	
		NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | (1U<<20);      //Make Pause/Start Switches of priority 1 (Lower Priority than door latch)
	
		// Interrupt initialization for port A
		NVIC_EN0_R 	 |= 1U<<0; 				 //Enabling PORTA Interrupt 
		GPIO_PORTA_IS_R  &=	~IR;      			 //Interrupt sense   0 for edge && 1 for level
		GPIO_PORTA_IBE_R &= ~IR;  		 		 //Interrupt both edges 0 Edge controlled by GPIOIEV && 1 Both edges
		GPIO_PORTA_IEV_R |= IR;    			         //Interrupt Event 0 Falling && 1 Rising
		GPIO_PORTA_ICR_R |= IR;   		 		 //Clear any prior interrupt
		GPIO_PORTA_IM_R  |= IR;  		 	         //Interrupt Mask  1 Enable Interrupt &&  0 Disable interrupt
		NVIC_PRI0_R = (NVIC_PRI0_R & 0xFF00FFFF) | (0U<<20); 	 //Make Door Latch of priority 0 (Highest priority)
}

void GPIOF_Handler(void)
{
	if (GPIO_PORTF_MIS_R&SW1) 		//PAUSE/RESUME ACTION
	{
		GPIO_PORTF_ICR_R |= SW1;  	// Clear Interrupt
		if (state == IDLE)			// no input was specified by user
	    {
			state = IDLE;
	    }
		else if (state == COOKING)	// user decided to pause cooking
		{			
			SysTick_Wait1ms(1000); 	// To handle the SysTickWait delay in LCD_CountDown
			cookingTime[4] += 1;
			if (cookingTime[4]==':')
			{
				cookingTime[4]='0';
				cookingTime[3]++;										
			}
			if (cookingTime[3] == '6' && cookingTime[4] == '0') {cookingTime[1]++; cookingTime[3] = '0'; cookingTime[4] = '0';}
			state = PAUSED;
			while(Pause);
		}
		else if (state == INPUT || state == PAUSED)	// user decided to cancel input or to stop cooking
		{
			LCD_cmd(clearDisplay);
			IDLE_Msg;
			state = IDLE;
		}
		GPIO_PORTF_ICR_R |= SW1;  	// Clear Interrupt
	}
				
	if ((GPIO_PORTF_MIS_R & SW2)) 	// COOK ACTION
	{
		GPIO_PORTF_ICR_R |= SW2;  	// Clear Interrupt
		if (state == IDLE)			// no input was specified from user
		{
			IDLE_Msg;
			state  = IDLE;
		}
		else if (state == INPUT)	// user decided to start cooking
		{
			if (doorClosed)
				state = COOKING;
			else
			{
				Close_Door_Msg;
				while (!doorClosed);
			}
		}						
		else if (state == PAUSED)	// user decided to continue cooking
		{	
			if (doorClosed)
				state = COOKING;
			else
			{
				Close_Door_Msg;
				while (!doorClosed);
			}
		}
		GPIO_PORTF_ICR_R |= SW2;  	// Clear Interrupt
	}
}


void GPIOA_Handler(void)
{
	if (GPIO_PORTA_MIS_R&IR)
	{	
		GPIO_PORTA_ICR_R |= IR;	// Clear Interrupt
		if (state == COOKING  && (!(doorClosed)))
		{	
			state = PAUSED;
		}
		else if (state == INPUT && (doorClosed))
		{
			LCD_cmd(clearDisplay);
			if(doorClosed) SW2_Msg;
			SysTick_Wait1ms(1000);
			if (Input == 'D')
			{
			    LCD_cmd(clearDisplay);
			    LCD_cmd(returnHome);
			    dearrange(cookingTime);
			    LCD_WriteString(cookingTime);
			    Bluetooth_SendString(cookingTime);
			    Bluetooth_SendChar('\n');
			    rearrange(cookingTime);
			}
		}
		GPIO_PORTA_ICR_R |= IR;  // Clear Interrupt
	}
}

