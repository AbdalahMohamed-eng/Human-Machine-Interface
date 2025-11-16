/************************************************************************************
 * Module: Fan
 * File Name: Fan.h
 * Description: Header file for Fan control
 * Author: Amr Ahmed 
 ************************************************************************************/
#ifndef FAN_H_
#define FAN_H_

#include <avr/io.h>
#include "macros.h" 

// Initialize fan pin
void Fan_Init(void);

// Turn fan ON
void Fan_On(void);

// Turn fan OFF
void Fan_Off(void);

#endif
