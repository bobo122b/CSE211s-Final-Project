#ifndef SWITCHES_H
#define SWITCHES_H

#define SW1 1U<<4   //SW1: F4
#define SW2 1U<<0  //SW2: F0
#define SW3 1U<<7  //SW3: A7

#define red 1U<<0
#define blue 1U<<1
#define green 1U<<2

#define SW1_Pressed (!(GPIO_PORTA_DATA_R &SW1))
#define SW2_Pressed (!(GPIO_PORTA_DATA_R &SW2))
#define SW3_Pressed (!(GPIO_PORTA_DATA_R &SW3))


void Switches_Init(void);

#endif