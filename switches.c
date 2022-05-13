#include "switches.h"
#include "tm4c123gh6pm.h"
#include "stdint.h"

void Switches_Init(void)
{
		SYSCTL_RCGCGPIO_R |=0x20;
		while ((SYSCTL_PRGPIO_R&0x20)==0){}
		GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
		GPIO_PORTF_CR_R = red|blue|green|SW1|SW2;
		GPIO_PORTF_DEN_R |= red|blue|green|SW1|SW2;
		GPIO_PORTF_DIR_R |= red|green|blue;
		GPIO_PORTF_PUR_R |= SW1|SW2; //SW1, SW2 input
		GPIO_PORTF_AMSEL_R = 0;
		GPIO_PORTF_AFSEL_R = 0;
		GPIO_PORTF_PCTL_R = 0;
			
		SYSCTL_RCGCGPIO_R |=0x01;
		while ((SYSCTL_PRGPIO_R&0x01)==0){}
		GPIO_PORTA_LOCK_R = GPIO_LOCK_KEY;
		GPIO_PORTA_CR_R |= SW3;	
		GPIO_PORTA_DEN_R |= SW3;
		GPIO_PORTA_DIR_R &= ~(SW3); //SW3 input
		GPIO_PORTA_PUR_R |= SW3;
		GPIO_PORTA_AMSEL_R = 0;
		GPIO_PORTA_AFSEL_R = 0;
		GPIO_PORTA_PCTL_R = 0;			
}
