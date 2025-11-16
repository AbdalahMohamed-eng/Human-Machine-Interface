/************************************************************************************
 * Module: LM35 Temperature Sensor
 * File Name: LM35.h
 * Description: Header file for LM35 temperature sensor driver
 * Author:  Youssef Alaa 
 ************************************************************************************/
#ifndef LM35_H_
#define LM35_H_
#include <avr/io.h>
#include "std_types.h"
#include "macros.h"
/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
// set up ADC for LM35
void LM35_Init(void);
// read current temperature (C) from LM35
float32 LM35_Read(void);
#endif 
