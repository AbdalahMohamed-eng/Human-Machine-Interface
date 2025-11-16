/************************************************************************************
 * Module: LM35 Temperature Sensor
 * File Name: LM35.c
 * Description: Implementation of LM35 driver
 * Author: Youssef Alaa 
 ************************************************************************************/

#include <avr/io.h>
#include "LM35.h"
#include "macros.h"

#define LM35_CALIBRATION 0.78f  // calibration factor

void LM35_Init(void)
{
    // Set reference to AVCC and select ADC0
    SET_BIT(ADMUX, REFS0);
    CLEAR_BIT(ADMUX, REFS1);

    // Enable ADC, prescaler = 128 → 16MHz / 128 = 125kHz
    SET_BIT(ADCSRA, ADEN);
    SET_BIT(ADCSRA, ADPS2);
    SET_BIT(ADCSRA, ADPS1);
    SET_BIT(ADCSRA, ADPS0);
}

float32 LM35_Read(void)
{
    uint16 sum = 0;
    const uint8 samples = 8;

    for (uint8 i = 0; i < samples; i++)
    {
        ADMUX &= 0xF0;          // select ADC0
        SET_BIT(ADCSRA, ADSC);  // start conversion
        while (BIT_IS_SET(ADCSRA, ADSC));
        sum += ADC;
    }

    float32 avg_adc = sum / (float32)samples;
    float32 voltage = (avg_adc * 5.0f) / 1024.0f;
    float32 temperatureC = voltage * 100.0f; // 10mV/°C

    return temperatureC * LM35_CALIBRATION; // calibrated temperature
}
