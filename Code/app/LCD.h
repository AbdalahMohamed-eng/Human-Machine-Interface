/************************************************************************************
 * Module: LCD
 * File Name: LCD.h
 * Description: Header file for 16x2 LCD driver (4-bit mode)
 * Author: Amr Ahmed
 ************************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include "std_types.h"
#include "macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define LCD_Data_Dir   DDRA
#define LCD_Data_Port  PORTA
#define LCD_CTRL_Dir   DDRH
#define LCD_CTRL_Port  PORTH
#define LCD_RS         PH5
#define LCD_EN         PH6
/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
// init LCD 
void LCD_Init(void);

// send command to LCD 
void LCD_Command(uint8 cmnd);

// display a single character
void LCD_Char(uint8 data);

// print a full string
void LCD_String(char *str);

// display text at a specific row and column
void LCD_String_xy(uint8 row, uint8 pos, char *str);

// Clear Screen 
void LCD_Clear(void);

// clear one line (0 or 1)
void LCD_ClearLine(uint8 line);

#endif 
