/************************************************************************************
 * Module: Display
 * File Name: Display.h
 * Description: LCD display functions for temperature and limits
 * Author: Youssef Alaa
 ************************************************************************************/
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "LCD.h"

/* External variables */
extern float last_temp;
extern float low_limit;
extern float high_limit;
extern char buffer[32];

/* Function Prototypes */
void update_temp_display(void);
void update_limits_display(void);

#endif /* DISPLAY_H_ */
