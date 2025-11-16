/************************************************************************************
 * Module: Buttons
 * File Name: Buttons.c
 * Description: Handles push button setup and reading
 * Author: Abdalah Mohamed 
 ************************************************************************************/
#include "Buttons.h"
#include <util/delay.h>

#define DEBOUNCE_DELAY 20  // ms

void Buttons_Init(void)
{
    // configure PC0–PC3 as input
    CLEAR_BIT(DDRC, PC0);
    CLEAR_BIT(DDRC, PC1);
    CLEAR_BIT(DDRC, PC2);
    CLEAR_BIT(DDRC, PC3);

    // enable internal pull-ups
    SET_BIT(PORTC, PC0);
    SET_BIT(PORTC, PC1);
    SET_BIT(PORTC, PC2);
    SET_BIT(PORTC, PC3);
}

uint8 Button_High_Inc_Pressed(void)
{
    if (BIT_IS_CLEAR(PINC, PC0)) { _delay_ms(DEBOUNCE_DELAY); if (BIT_IS_CLEAR(PINC, PC0)) return 1; }
    return 0;
}

uint8 Button_High_Dec_Pressed(void)
{
    if (BIT_IS_CLEAR(PINC, PC2)) { _delay_ms(DEBOUNCE_DELAY); if (BIT_IS_CLEAR(PINC, PC2)) return 1; }
    return 0;
}

uint8 Button_Low_Inc_Pressed(void)
{
    if (BIT_IS_CLEAR(PINC, PC1)) { _delay_ms(DEBOUNCE_DELAY); if (BIT_IS_CLEAR(PINC, PC1)) return 1; }
    return 0;
}

uint8 Button_Low_Dec_Pressed(void)
{
    if (BIT_IS_CLEAR(PINC, PC3)) { _delay_ms(DEBOUNCE_DELAY); if (BIT_IS_CLEAR(PINC, PC3)) return 1; }
    return 0;
}
