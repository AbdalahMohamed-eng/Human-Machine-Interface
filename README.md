# Temperature Monitoring & Control System  
Embedded Systems Coursework 2 – Arduino Mega Project

## 📌 Group Members
- **Abdullah Mohamed**
- **Amr Ahmed**
- **Youssef Alaa**

**Status:** Final Draft  
**Version:** 1.0  
**Date:** 12/11/2025  

---

## 📖 Introduction
This project implements a temperature monitoring and control system using an **Arduino Mega**, **LM35 sensor**, **16×2 LCD**, **LED indicators**, **buzzer**, and **cooling fan**. The system allows the user to configure **High Limit (HL)** and **Low Limit (LL)** temperature thresholds using four push buttons.  

Based on the measured temperature:
- If temperature is **below LL** → Blue LED ON  
- If temperature is **above HL** → Red LED + Fan + Buzzer ON  
- If temperature is **between LL and HL** → All indicators OFF  

Temperature readings are updated every **2 seconds**, while user input is read every **50 ms**.

---

## 🎯 Purpose
The system demonstrates:
- Sensor interfacing (LM35)
- LCD-based UI
- Button-driven configuration
- Temperature-based actuation (LEDs, Fan, Buzzer)
- Proper power distribution using an external 8V battery and 5V regulator
- Layered, modular embedded firmware

---

## 📌 Scope
This project includes:
- Real-time temperature sensing
- Adjustable HL & LL temperature thresholds
- Visual, audible, and mechanical temperature-response mechanisms
- Modular HAL drivers (LCD, Fan, LM35, Buttons, LEDs)
- Clean layered architecture (Application → HAL → Utilities)

The project **does not** include data logging, wireless communication, PID control, or multi-sensor support.

---
============================================================
               SYSTEM ARCHITECTURE
============================================================

                 ┌─────────────────────────┐
                 │   APPLICATION LAYER     │
                 │       (app.ino)         │
                 │------------------------ │
                 │ - Main loop             │
                 │ - Temperature read      │
                 │ - Button handling       │
                 │ - Control LEDs/Fan      │
                 │ - Update LCD display    │
                 └───────────┬──────────── ┘
                             │
                             │ uses
                             ▼
       ┌─────────────────────────────────────────────┐
       │       HAL LAYER (Hardware Abstraction)      │
       │---------------------------------------------│
       │ ┌──────────────┐ ┌──────────────┐           │
       │ │ LCD Driver   │ │ LED Driver   │           │
       │ │ LCD.h/.ino   │ │ LED.h/.ino   │           │
       │ └──────────────┘ └──────────────┘           │
       │ ┌──────────────┐ ┌──────────────┐           │
       │ │ Fan Driver   │ │ LM35 Driver  │           │
       │ │ Fan.h/.ino   │ │ LM35.h/.ino  │           │
       │ └──────────────┘ └──────────────┘           │
       │ ┌──────────────┐ ┌──────────────┐           │
       │ │ Buttons      │ │ Display      │           │
       │ │ Driver       │ │ Module       │           │
       │ │ Buttons.h/.ino│ Display.h/.ino│           │
       │ └──────────────┘ └──────────────┘           │
       └───────────┬───────────────────┬───────────  ┘
                   │                   │
                   │ uses              │ uses
                   ▼                   ▼
         ┌─────────────────┐   ┌─────────────────┐
         │  LEDs/Fan/Buzzer│   │  LCD Display    │
         └─────────────────┘   └─────────────────┘

                             ▲
                             │
                             │ depends on
                             │
       ┌─────────────────────────────────────────────┐
       │           UTILITIES LAYER                   │
       │---------------------------------------------│
       │ • macros.h   → Bitwise operations           │
       │ • std_types.h → Standard types (uint8 etc.) │
       └─────────────────────────────────────────────┘

============================================================
                      DATA / CONTROL FLOW
============================================================

LM35 Sensor  ──>  LM35 Driver  ──┐
Buttons      ──>  Buttons Driver ─┤
                                 ▼
                          Application Layer
                                 │
         ┌───────────────────────┴───────────────────────┐
         │                                               │
     Control Outputs                                   Display
     ┌───────────────┐                                ┌─────────────┐
     │ Red LED       │                                │ LCD Screen  │
     │ Blue LED      │                                └─────────────┘
     │ Fan           │
     │ Buzzer        │
     └───────────────┘

============================================================

---

## ✔️ System Behavior

### Temperature Response Table

| Condition | System Action |
|----------|---------------|
| **Temp ≥ HL** | Red LED ON, Fan ON, Buzzer ON |
| **Temp ≤ LL** | Blue LED ON, Fan OFF |
| **LL < Temp < HL** | All indicators OFF |
| **Buttons Pressed** | Adjust HL/LL values and update LCD |

### LCD Display Format
Temp: XX.X C
HL:XX LL:YY



---

## 🔧 Hardware Components
| Component | Purpose |
|----------|---------|
| Arduino Mega 2560 | Main controller |
| LM35 Sensor | Temperature measurement |
| 16×2 LCD | Display temperature + limits |
| 4 Push Buttons | Adjust HL / LL |
| Red & Blue LEDs | High/Low temperature indication |
| DC Fan + Driver (Transistor/MOSFET + Diode) | Cooling system |
| Buzzer | Alarm for high temperature |
| 8V Battery + 5V Regulator | Power |
| Potentiometer | LCD contrast |
| Breadboard + Wires | Prototyping |

---

## 🔌 Circuit Connections

### Inputs
| Component | Arduino Pin |
|----------|-------------|
| LM35 Output | PA0 / A0 (ADC0) |
| HL+ Button | PC0 |
| HL– Button | PC1 |
| LL+ Button | PC2 |
| LL– Button | PC3 |

### Outputs
| Component | Pin |
|----------|-----|
| Red LED (High Temp) | PC4 |
| Blue LED (Low Temp) | PC5 |
| Fan Control | PB4 |
| Buzzer | (Your chosen pin) |
| LCD Data Lines | PORTA (upper nibble) |
| LCD RS | PH5 |
| LCD EN | PH6 |

---

## s Software Overview

###  Temperature Measurement
- LM35 → ADC0
- Conversion:  
  `voltage = (ADC_value * 5.0) / 1024`  
  `tempC = voltage * 100`

###  Button Handling
- Internal pull-ups enabled  
- Press = LOW  
- Buttons modify HL/LL with safety rules:
  - `HL ≤ 100`  
  - `LL ≥ 0`  
  - `LL < HL`


### Control Logic (Executed Every 2 Seconds)

- High Temperature (Temp ≥ HL):
    - Red LED ON
    - Fan ON
    - Buzzer ON

- Low Temperature (Temp ≤ LL):
    - Blue LED ON
    - Red LED OFF
    - Fan OFF
    - Buzzer OFF

- Normal Temperature (LL < Temp < HL):
    - All indicators OFF
    - Fan OFF
    - Buzzer OFF

---

### Main Loop Tasks

1. Update software timer.
2. Read LM35 sensor every 2 seconds to get the current temperature.
3. Read buttons every 50 ms to detect limit adjustments.
4. Update the LCD display with:
    - Current Temperature
    - High Limit (HL)
    - Low Limit (LL)

---

### System Features

Feature                 | Description
------------------------|-------------------------------------------------------
Real-time Monitoring    | LM35 sensor with periodic sampling every 2 seconds
Adjustable Limits       | Four push buttons with safe boundaries for HL and LL
Visual Indicators       | Red LED for high temp, Blue LED for low temp
Audio Indicator         | Buzzer activated when temperature exceeds HL
Cooling System          | Fan automatically activated when Temp ≥ HL
Modular Architecture    | HAL layer separates hardware drivers from application logic
Power Management        | External 5V regulator prevents overload of Arduino and fan

