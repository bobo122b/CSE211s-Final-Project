#include "Microwave.h"

#define E4 1U<<4
#define E5 1U<<5

void Bluetooth_Init(void)
{
    SYSCTL_RCGCUART_R |= 0x20;              // enable clock to UART5
    SYSCTL_RCGCGPIO_R |= 0x10;              // enable clock to PORTE for PE4/Rx and RE5/Tx
    Delay(1);

    // UART5 initialization 
    UART5_CTL_R  = 0;                       // UART5 module disbable 
    UART5_IBRD_R = 520;                     // for 9600 baud rate, integer = 520    (int)(80000000/(16*9600))
    UART5_FBRD_R = 53;                      // for 9600 baud rate, fractional = 53  (int)((80000000/(16.0f*9600) - (int)(80000000/(16*9600)) * 64)
    UART5_CC_R   = 0;                       // select system clock
    UART5_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);                    // data lenght 8-bit, FIFO on
   // UART5_IFLS_R &= ~0x3F;
   // UART5_IFLS_R += (UART_IFLS_TX1_8|UART_IFLS_RX1_8);
   // UART5_IM_R |= (UART_IM_RXIM|UART_IM_RTIM);
    UART5_CTL_R  = 0x301;                   // Enable UART5 module, Rx and Tx

    // UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function
    GPIO_PORTE_DEN_R   |= 0x30;              // set PE4 and PE5 as digital
    GPIO_PORTE_AFSEL_R |= 0x30;              // Use PE4,PE5 alternate function
    GPIO_PORTE_AMSEL_R &= ~(0x30);           // Turn off analog function
    GPIO_PORTE_PCTL_R   = (GPIO_PORTE_PCTL_R & 0xFF00FFFF) + 0x00110000;         // configure PE4 and PE5 for UART

    // enable interrupt
    //NVIC_EN1_R  |= NVIC_EN1_INT_M;
    //UART5_ICR_R &= ~(0x0780);
}

char Bluetooth_ReceiveChar(void)
{
      char data;
      GPIO_PORTC_DATA_R |= 0xF0;
      while((UART5_FR_R & (1<<4)) != 0) {   // wait until Rx buffer is not empty
          if(Pause)                         
              return '~';                   // return '~' if SW1 is pressed
          else if (Start)
              return 's';                   // return 's' if SW2 is pressed
          else if (Keypad_pressed)
          {
              Keypad_pressed = 0;
              return(Keypad_getKey());
          }
      }
    data = UART5_DR_R; 			// before giving it another byte
		data = (unsigned char) data;
}

void Bluetooth_SendChar(unsigned char data)
{
    while((UART5_FR_R & (1<<5)) != 0);      // wait until Tx buffer not full
    UART5_DR_R = data;                      // before giving it another byte
}

void Bluetooth_SendString(char *str)
{
  while(*str)
    {
      Bluetooth_SendChar(*(str++));         // send the string character by character
    }
}

void Delay(unsigned long counter)           // generic delay function only used here
{
    unsigned long i = 0;

    for(i=0; i< counter; i++);
}


