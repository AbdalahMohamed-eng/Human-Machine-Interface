/************************************************************************************
 * Module: Fan
 * File Name: Fan.c
 * Description: Handles Fan setup and control
 * Author: Amr Ahmed
 ************************************************************************************/
#include "Fan.h"

// Fan is connected to PB4
// On Arduino MEGA, PB4 = Digital Pin 10

void Fan_Init(void) {
    SET_BIT(DDRB, PB4);    // Set PB4 as output
    CLEAR_BIT(PORTB, PB4); // Ensure fan is OFF initially
}

void Fan_On(void) {
    SET_BIT(PORTB, PB4);   // PB4 HIGH → Fan ON
}

void Fan_Off(void) {
    CLEAR_BIT(PORTB, PB4); // PB4 LOW → Fan OFF
}
