#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "LCD.h"
#include "LED.h"
#include "LM35.h"
#include "Buttons.h"
#include "Display.h"
#include "Fan.h"
#include "macros.h"   

// ---------------- Configuration ----------------
#define HYSTERESIS 0.5f           // ±0.5°C for hysteresis
#define TEMP_SAMPLES 8            // ADC averaging
#define TEMP_READ_INTERVAL 2000   // 2s

// ---------------- Global Variables ----------------
float last_temp = 0.0;
float high_limit = 30.0;
float low_limit  = 20.0;
float calibration_factor = 0.78f; // LM35 calibration factor
char buffer[32];

// ---------------- Button counters ----------------
uint16 highIncCounter = 0;
uint16 highDecCounter = 0;
uint16 lowIncCounter  = 0;
uint16 lowDecCounter  = 0;

// ---------------- Alert state ----------------
uint8 highAlert = 0;
uint8 lowAlert  = 0;

// ---------------- Function Prototypes ----------------
void readTemperature(void);
void updateAlerts(void);
void handleButtons(void);

void setup(void) {
    // Initialize peripherals
    LCD_Init();
    LED_Init();
    LM35_Init();
    Buttons_Init();
    Fan_Init();

    // Display initial temperature and limits
    update_temp_display();
    update_limits_display();
}

void loop(void) {
    static uint32 elapsed_time = 0;

    // ---------------- Temperature Reading ----------------
    if (elapsed_time >= TEMP_READ_INTERVAL) {
        readTemperature();
        update_temp_display();
        updateAlerts();
        elapsed_time = 0;
    }

    // ---------------- Buttons Handling ----------------
    handleButtons();

    _delay_ms(50);  // polling delay
    elapsed_time += 50;
}

// ---------------- Read Temperature with Averaging ----------------
void readTemperature(void) {
    uint16 sum = 0;
    for (uint8 i = 0; i < TEMP_SAMPLES; i++) {
        ADMUX &= 0xF0;          // select ADC0
        SET_BIT(ADCSRA, ADSC);  // start conversion
        while (BIT_IS_SET(ADCSRA, ADSC));
        sum += ADC;
    }
    float avg_adc = sum / (float)TEMP_SAMPLES;
    float voltage = (avg_adc * 5.0f) / 1024.0f;
    last_temp = voltage * 100.0f * calibration_factor;
}

// ---------------- Update LEDs and Fan with Hysteresis ----------------
void updateAlerts(void) {
    if (last_temp > high_limit + HYSTERESIS) {
        LED_On_High();
        LED_Off_Low();
        Fan_On();
        highAlert = 1;
        lowAlert  = 0;
    }
    else if (last_temp < low_limit - HYSTERESIS) {
        LED_Off_High();
        LED_On_Low();
        Fan_Off();
        highAlert = 0;
        lowAlert  = 1;
    }
    else {
        LED_Off_High();
        LED_Off_Low();
        Fan_Off();
        highAlert = 0;
        lowAlert  = 0;
    }
}

// ---------------- Buttons Handling ----------------
void handleButtons(void) {
    // --- High limit increase ---
    if (Button_High_Inc_Pressed()) {
        highIncCounter += 50;
        high_limit += 1.0;  // increase on short or long press
        if (high_limit > 100.0) high_limit = 100.0;
        if (low_limit >= high_limit) low_limit = high_limit - 1.0;
        update_limits_display();
        if (highIncCounter >= 500) highIncCounter = 0; // long press reset
    } else {
        highIncCounter = 0;
    }

    // --- High limit decrease ---
    if (Button_High_Dec_Pressed()) {
        highDecCounter += 50;
        high_limit -= 1.0;
        if (high_limit < low_limit + 1.0) high_limit = low_limit + 1.0;
        update_limits_display();
        if (highDecCounter >= 500) highDecCounter = 0;
    } else {
        highDecCounter = 0;
    }

    // --- Low limit increase ---
    if (Button_Low_Inc_Pressed()) {
        lowIncCounter += 50;
        low_limit += 1.0;
        if (low_limit >= high_limit) low_limit = high_limit - 1.0;
        update_limits_display();
        if (lowIncCounter >= 500) lowIncCounter = 0;
    } else {
        lowIncCounter = 0;
    }

    // --- Low limit decrease ---
    if (Button_Low_Dec_Pressed()) {
        lowDecCounter += 50;
        low_limit -= 1.0;
        if (low_limit < 0.0) low_limit = 0.0;
        update_limits_display();
        if (lowDecCounter >= 500) lowDecCounter = 0;
    } else {
        lowDecCounter = 0;
    }
}
