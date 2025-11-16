/************************************************************************************
 * Module: LCD
 * File Name: LCD.c
 * Description: 16x2 LCD driver (4-bit mode)
 * Author: Amr Ahmed
 ************************************************************************************/

#include "LCD.h"
#include <util/delay.h>

// send command to LCD 
void LCD_Command(uint8 cmnd)
{
    LCD_Data_Port = (LCD_Data_Port & 0x0F) | (cmnd & 0xF0);
    CLEAR_BIT(LCD_CTRL_Port, LCD_RS);
    SET_BIT(LCD_CTRL_Port, LCD_EN);
    _delay_us(1);
    CLEAR_BIT(LCD_CTRL_Port, LCD_EN);
    _delay_us(200);

    LCD_Data_Port = (LCD_Data_Port & 0x0F) | (cmnd << 4);
    SET_BIT(LCD_CTRL_Port, LCD_EN);
    _delay_us(1);
    CLEAR_BIT(LCD_CTRL_Port, LCD_EN);
    _delay_ms(2);
}

// display a single character
void LCD_Char(uint8 data)
{
    LCD_Data_Port = (LCD_Data_Port & 0x0F) | (data & 0xF0);
    SET_BIT(LCD_CTRL_Port, LCD_RS);
    SET_BIT(LCD_CTRL_Port, LCD_EN);
    _delay_us(1);
    CLEAR_BIT(LCD_CTRL_Port, LCD_EN);
    _delay_us(200);

    LCD_Data_Port = (LCD_Data_Port & 0x0F) | (data << 4);
    SET_BIT(LCD_CTRL_Port, LCD_EN);
    _delay_us(1);
    CLEAR_BIT(LCD_CTRL_Port, LCD_EN);
    _delay_ms(2);
}

/* init LCD */
void LCD_Init(void)
{
    LCD_Data_Dir = 0xFF;
    SET_BIT(LCD_CTRL_Dir, LCD_RS);
    SET_BIT(LCD_CTRL_Dir, LCD_EN);

    _delay_ms(20);
    LCD_Command(0x02); // 4-bit mode
    LCD_Command(0x28); // 2 lines, 5x7 font
    LCD_Command(0x0C); // Display ON, cursor OFF
    LCD_Command(0x06); // Increment cursor
    LCD_Command(0x01); // Clear display
    _delay_ms(2);
}

/* display string on LCD */
void LCD_String(char *str)
{
    uint8 i = 0;
    while (str[i] != '\0') LCD_Char(str[i++]);
}

// display text at a specific row and column
void LCD_String_xy(uint8 row, uint8 pos, char *str)
{
    if (row == 0) LCD_Command((pos & 0x0F) | 0x80);
    else          LCD_Command((pos & 0x0F) | 0xC0);

    LCD_String(str);
}

/* Clear entire display */
void LCD_Clear(void)
{
    LCD_Command(0x01);
    _delay_ms(2);
    LCD_Command(0x80);
}

/* Clear one line (0 or 1) */
void LCD_ClearLine(uint8 line)
{
    LCD_Command((line == 0) ? 0x80 : 0xC0);
    for (uint8 i = 0; i < 16; i++) LCD_Char(' ');
    LCD_Command((line == 0) ? 0x80 : 0xC0);
}
