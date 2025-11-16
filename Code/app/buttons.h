/************************************************************************************
 * Module: Buttons
 * File Name: Buttons.h
 * Description: Header file for push button interface
 * Author: Abdalah Mohamed 
 ************************************************************************************/
#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <avr/io.h>
#include "std_types.h"
#include "macros.h"

/* Prototypes */
void Buttons_Init(void);
uint8 Button_High_Inc_Pressed(void);
uint8 Button_High_Dec_Pressed(void);
uint8 Button_Low_Inc_Pressed(void);
uint8 Button_Low_Dec_Pressed(void);

#endif

