/************************************************************************************
 * Module: LED
 * File Name: LED.h
 * Description: Header file for LED control driver
 * Author: Abdalah Mohamed 
 ************************************************************************************/
#ifndef LED_H_
#define LED_H_
#include <avr/io.h>
#include "std_types.h"
#include "macros.h"
/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
// set up LED pins
void LED_Init(void);

// high-limit LED control
void LED_On_High(void);
void LED_Off_High(void);

// low-limit LED control
void LED_On_Low(void);
void LED_Off_Low(void);

#endif 
