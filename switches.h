/*************************************************************************************************
 *!
 * @file 		switches.h
 * @authors  	Ahmed Shafeek, Abdelrahman Mostafa, Ahmed Hamdy, Muhammed Emad, Abdelrahman Sameh 
 * @brief		contains the functions that defines the behaviour of the switches.
**************************************************************************************************/
#ifndef SWITCHES_H
#define SWITCHES_H

#define SW1     1U<<4   //SW1: F4   Pause
#define SW2     1U<<0   //SW2: F0   Start
#define IR      1U<<7   //SW3: A7   Door

#define red     1U<<1   // red   led PF1
#define blue    1U<<2   // blue  led PF2
#define green   1U<<3   // green led PF3

#define SW1_Pressed (!(GPIO_PORTF_DATA_R &SW1))     // defines if switch 1 is pressed or not
#define SW2_Pressed (!(GPIO_PORTF_DATA_R &SW2))     // defines if switch 2 is pressed or not
#define IR_On       (!(GPIO_PORTA_DATA_R & IR))     // defines if IR senses the door or not

#define Input_First_Msg LCD_cmd(clearDisplay);\
LCD_cmd(cursorOff);\
LCD_WriteString("Err");\
LCD_cmd(SecondLine);\
LCD_WriteString("Input first");\
Bluetooth_SendString("Err\nInput first\n")

#define Close_Door_Msg LCD_cmd(clearDisplay);\
LCD_cmd(cursorOff);\
gotoxy(1,0);\
LCD_WriteString("Close the door");\
Bluetooth_SendString("Close the door\n")

#define IDLE_State_Msg LCD_cmd(clearDisplay);\
LCD_cmd(cursorOff);\
LCD_WriteString("IDLE state");\
Bluetooth_SendString("IDLE state\n")


/*!
 * @fn      Switches_Init
 * @brief   Used to initialize SW1, SW2, and SW3 pins as digital input with PUR
*/
void Switches_Init(void);
/*!
 * @fn      GPIOF_Handler
 * @brief   Used to handle the interrupts of port F
*/
void GPIOF_Handler(void);
/*!
 * @fn      GPIOA_Handler
 * @brief   Used to handle the interrupts of port A
*/
void GPIOA_Handler(void);

#endif
