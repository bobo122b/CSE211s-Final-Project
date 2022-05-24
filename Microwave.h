/***************************************************************************************************
*!
 * @file 	Microwave.h
 * @authors Abdelrahman Mostafa, Ahmed Shafeek, Ahmed Hamdy, Muhammed Emad, Abdelrahman Sameh
 * @brief	contains the main macros and functions and definitions and the enum of the FSM
****************************************************************************************************/
#ifndef MICROWAVE_H_
#define MICROWAVE_H_
/*!
 LCD: 	
 	D4:D7 	==> B4:B7
	RS 		==> A5
	E 		==> A6
	RW		==> GND
	VSS		==> GND
	TivaGND ==> GND
	VDD 	==> 5V
	V0 		==> POT

 BUZZER: 	
 		A2
 external SWITCH: 	
 		A7

 KEYPAD:
 	Cols C4,C5,C6,C7  
	Rows E0,E1,E2,E3 
				
*/

// Adding all needed files to be included in one file
#include "Bluetooth.h"
#include "tm4c123gh6pm.h"
#include "lcd.h"
#include "keypad.h"
#include "SysTick.h"
#include "switches.h"
#include "Timer.h"
#include "Bluetooth.h"
#include "Stepper.h"
#include <stdint.h>

// definitions of used macros in main function

#define doorClosed 	IR_On   		// defines whether door is closed
#define Start 		SW2_Pressed		// defines whether the user chose to start cooking or not
#define Pause 		SW1_Pressed 	// defines whether the user chose to cancel/pause cooking or not


#define not_valid_num(n)   (n > '9' || n < '0')						// a valid input number from 0 to 9
#define not_valid_input(n) (n!='A' && n!='B' && n!='C' && n!='D')	// a valid input character from A to D

// Cooking is done when the time array is equal to "00:00"
#define CookingIsDone (cookingTime[0] == '0' && cookingTime[1] == '0' && cookingTime[3] == '0' && cookingTime[4] == '0')

#define not_valid_CookingTime(t) ( not_valid_num(t[0]) || not_valid_num(t[1]) || not_valid_num(t[3]) || not_valid_num(t[4]) || \
(t[0] > '3' || t[0] < '0') || /* 10 * mins maximum = 3 */ \
(t[1] > '9' || t[1] < '0') || /* 1  * mins maximum = 9 */ \
(t[3] > '5' || t[3] < '0') || /* 10 * sec  maximum = 5 */ \
(t[4] > '9' || t[4] < '0') || /* 1  * sec  maximum = 9 */ \
(t[0] == '3' && (t[1] > '0' || t[3] > '0' || t[4] > '0' )) || /* maximum = 30:00 */\
(CookingIsDone))			/* minimum = 00:01 */


// rearranges the given array and makes it ready for next input
// ex: "00:05" becomes "00:50", and arr[4] becomes ready for input
#define rearrange(arr) arr[0] = arr[1];\
arr[1] = arr[3];\
arr[3] = arr[4];\
arr[5] = 0

// dearranges the given array and makes it ready for evaluation of time array
// ex: "00:50" becomes "00:05", and arr becomes ready for evaluation
#define dearrange(arr) arr[4] = arr[3];\
arr[3] = arr[1];\
arr[1] = arr[0];\
arr[0] = '0';\
arr[5] = 0

// fills the first array with the second array
// ex: fill_array(cookingTime, "00:00") fills cookingTime array with the value "00:00"
#define fill_array(arr1,arr2) arr1[0]=arr2[0];\
arr1[1]=arr2[1];\
arr1[2]=arr2[2];\
arr1[3]=arr2[3];\
arr1[4]=arr2[4];\
arr1[5]=0

#define buzzer 			(1U<<2)   												// Pin A2 for buzzer
#define Buzzer_Init() 	GPIO_PORTA_DEN_R |=buzzer; GPIO_PORTA_DIR_R |=buzzer	// initialize buzzer as a digital output

//*********************************************************************************************
//**********************IDLE STATE INITIALIZATION AND MESSAGES*********************************
//*********************************************************************************************
#define IDLE_Pins GPIO_PORTF_DATA_R &=~(red|blue|green);	/* Turn off all LEDs */\
GPIO_PORTA_DATA_R &=~buzzer;			/* Make sure buzzer is turned off */\
GPIO_PORTF_ICR_R  |= SW2|SW1;  			/* Clear interrupt of port F */\
GPIO_PORTA_ICR_R  |= IR;  				/* Clear interrupt of port A */\

#define IDLE_Msg LCD_cmd(clearDisplay);\
LCD_cmd(returnHome);\
LCD_cmd(cursorBlink);\
LCD_WriteString("Enter A,B,C,or D");\
LCD_cmd(SecondLine);\
LCD_WriteString("Input: ");\
Bluetooth_SendString("Enter A,B,C,or D\n")

//*********************************************************************************************
//***********************************INPUT MESSAGES********************************************
//*********************************************************************************************
#define Err_msg LCD_cmd(clearDisplay);\
gotoxy(7,0);\
LCD_WriteString("Err");\
gotoxy(2,1);\
LCD_WriteString("Enter 1 to 9\0");\
Bluetooth_SendString("Err\nEnter 1 to 9\n")

#define Popcorn_Msg LCD_cmd(cursorOff);\
gotoxy(4,0);\
LCD_WriteString("Popcorn");\
Bluetooth_SendString("Popcorn\n");\
SysTick_Wait1ms(1000);\
LCD_cmd(clearDisplay);\
gotoxy(2,1);\
LCD_WriteString("SW2 to start");\
Bluetooth_SendString("Enter s to start, p to cancel\n")

#define Beef_Msg gotoxy(2,0);\
LCD_WriteString("Beef Weight?");\
LCD_cmd(SecondLine);\
LCD_cmd(cursorBlink);\
Bluetooth_SendString("Beef Weight?\n");\
Bluetooth_SendString("Enter p to cancel\n")

#define Chicken_Msg LCD_WriteString("Chicken Weight ?");\
LCD_cmd(SecondLine);\
LCD_cmd(cursorBlink);\
Bluetooth_SendString("Chicken Weight?\n");\
Bluetooth_SendString("Enter p to cancel\n")

#define SW2_Msg LCD_cmd(clearDisplay);\
LCD_cmd(cursorOff);\
gotoxy(2,1);\
LCD_WriteString("SW2 to start");/* we tell the user that he needs to press on SW2 to start cooking */\
Bluetooth_SendString("Enter s to start, p to cancel\n")

#define Cooking_Time_Msg gotoxy(1,0);\
LCD_WriteString("Cooking time ?");\
LCD_cmd(cursorOff);\
SysTick_Wait1ms(1000);\
LCD_cmd(clearDisplay);\
LCD_cmd(cursorBlink);\
Bluetooth_SendString("Cooking time?\n");\
Bluetooth_SendString("Enter p to cancel\n")

#define Err_Cooking_Time LCD_cmd(clearDisplay);\
LCD_cmd(cursorOff);\
gotoxy(7,0);\
LCD_WriteString("Err");\
LCD_cmd(SecondLine);\
LCD_WriteString("00:01 to 30:00");\
Bluetooth_SendString("Err\n00:01 to 30:00\n")

#define Err_Invalid LCD_cmd(cursorOff);\
gotoxy(7,0);\
LCD_WriteString("Err");\
LCD_cmd(SecondLine);\
LCD_WriteString("Invalid input");\
Bluetooth_SendString("Err\nInvalid input\n")

//*********************************************************************************************
//**********************************COOKING MESSAGES*******************************************
//*********************************************************************************************
#define Blink_And_Buzz GPIO_PORTF_DATA_R ^= (red|green|blue);\
GPIO_PORTA_DATA_R ^=  buzzer;\
SysTick_Wait1ms(500)

#define Light_On GPIO_PORTF_DATA_R  |=  (red|green|blue)

#define Light_Off GPIO_PORTF_DATA_R &= ~(red|green|blue)

//*********************************************************************************************
//**********************************Paused MESSAGES********************************************
//*********************************************************************************************
#define Paused_Msg LCD_cmd(clearDisplay);\
LCD_cmd(cursorOff);\
gotoxy(5,0);\
LCD_WriteString(cookingTime);\
gotoxy(5,1);\
LCD_WriteString("PAUSED");\
if (!doorClosed)\
Bluetooth_SendString("PAUSED, Door is open\n");\
else \
Bluetooth_SendString("PAUSED\n")

#define Blink GPIO_PORTF_DATA_R ^=(red|green|blue);\
SysTick_Wait1ms(500)

/*!
 * @typedef     State
 * @c           IDLE    Ready for any input, door is closed.
 *                      can go to INPUT if any input occurs.
 * @c           INPUT   Taking input from user, door is closed.
 *                      can go to IDLE if user cancels.
 *                      can go to COOKING if user accepts specified input.
 * @c           COOKING Took input from user and accepted, door is closed.
 *                      can go to PAUSED if any interrupt occurs.
 *                      can go to IDLE if cooking is finished.
 * @c           PAUSED  Door is open, or was cooking and user clicked on pause button.
 *                      can go to COOKING if user continued (timer must be saved).
 *                      can go to IDLE if user clicked on end.
*/
 
typedef enum {IDLE, INPUT, COOKING, PAUSED} State;

//Extern Variables to be used by all files:
extern char cookingTime[6];		        // Used as the main cooking time to appear on the LCD and used in main function.
extern State state;				        // Machine state indicated by the State enum.
extern volatile int Keypad_pressed;		// defines if the keypad is pressed.
extern volatile char Input;             // To store input from key pad
extern char Weight;                     // Weight input in case of Beef of Chicken


// Functions definitions

/*!
 * @fn SystemInit
 * @brief This function includes all initializations in order
*/
void SystemInit(void);

/* INPUTS DESCRIPTION:
// A -> Popcorn -> display PopCorn and coock for 1 minute
// B -> beef    -> enter kilos from 1 to 9 -> 0.5min per kilo
// C -> chicken -> enter kilos from 1 to 9 -> 0.2min per kilo
// D -> take input time from user from right to left (00:00)
//      sw1(pf4) -> clear LCD 
//      sw2(pf0) -> start cooking
// sw3(ext) -> door latch
*/

/* CONDITIONS
// sw3(ext) -> pressed = door open -> no cooking
                         if cooking -> pause (keep timer on screen)
// sw2(pf0) -> pressed = start operation
//                       if paused -> continue cooking
// sw1(pf4) -> pressed = if first time -> pause (keep timer on screen)
//                       if second     -> clear time, stop cooking
// if cooking -> all leds on
// if finished cooking -> all leds flash 3 times with buzzer
*/
#endif // !MICROWAVE_H_
