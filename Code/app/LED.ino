/************************************************************************************
 * Module: LED
 * File Name: LED.c
 * Description: LED control driver
 * Author: Abdalah Mohamed 
 ************************************************************************************/
#include "LED.h"
// init LED pins
void LED_Init(void)
{
    SET_BIT(DDRC, PC4);
    SET_BIT(DDRC, PC5);
    CLEAR_BIT(PORTC, PC4);
    CLEAR_BIT(PORTC, PC5);
}
// High-limit LED control
void LED_On_High(void)  { SET_BIT(PORTC, PC4); }
void LED_Off_High(void) { CLEAR_BIT(PORTC, PC4); }
// Low-limit LED control
void LED_On_Low(void)   { SET_BIT(PORTC, PC5); }
void LED_Off_Low(void)  { CLEAR_BIT(PORTC, PC5); }
