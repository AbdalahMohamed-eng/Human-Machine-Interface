/************************************************************************************
 * Module: Display
 * File Name: Display.c
 * Description: LCD display functions for temperature and limits
 * Author: Youssef Alaa
 ************************************************************************************/
#include "Display.h"
#include <stdlib.h>

void update_temp_display(void)
{
    LCD_ClearLine(0);
    LCD_String("Temp:");
    dtostrf(last_temp, 5, 1, buffer);
    LCD_String(buffer);
    LCD_String("C ");

    if (last_temp >= low_limit && last_temp <= high_limit)
        LCD_String("   OK");
    else
        LCD_String(" NOK");
}

void update_limits_display(void)
{
    LCD_ClearLine(1);
    LCD_String("L:");
    dtostrf(low_limit, 4, 1, buffer);
    LCD_String(buffer);
    LCD_String(" H:");
    dtostrf(high_limit, 4, 1, buffer);
    LCD_String(buffer);
}
