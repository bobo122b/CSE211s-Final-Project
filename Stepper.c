#include "Stepper.h"
#include "SysTick.h"

void StepperInit(void)
{
		SYSCTL_RCGCGPIO_R |= 0x02;
		while ((SYSCTL_PRGPIO_R&0x02)==0){}
		GPIO_PORTB_LOCK_R = GPIO_LOCK_KEY;
		GPIO_PORTB_DEN_R |= 0x0F;
		GPIO_PORTB_DIR_R |= 0x0F;
}

void RotorStart(void)  //Rotate and delay (62 * 16ms = 1 second ) 
{
		volatile int i;
		for (i=0;i<62;i++)    
		{
					GPIO_PORTB_DATA_R = 0x08;
					SysTick_Wait1ms(4);
					GPIO_PORTB_DATA_R = 0x04;
					SysTick_Wait1ms(4);
					GPIO_PORTB_DATA_R = 0x02;
					SysTick_Wait1ms(4);
					GPIO_PORTB_DATA_R = 0x01;
					SysTick_Wait1ms(4);
		}
}