| **Group Memebrs**  | `Amr Ahmed, Youssef Alaa, Abdalah Mohamed` |
| :---------- | :------------------------------------------ |
| **Status**  | `CourseWork 2 Final Draft`                  |
| **Version** | `1.0`                                       |
| **Date**    | `12/11/2025`                                |

---
## Introduction

This project implements an embedded temperature-monitoring and control system using an Arduino Mega. The system reads ambient temperature through an LM35 sensor and displays the real-time value on a 16×2 LCD. Four push buttons allow the user to adjust the high-temperature (HL) and low-temperature (LL) limits. According to the configured range, the system provides immediate feedback using a blue LED when the temperature is below the low limit, and a red LED with a buzzer when it exceeds the high limit. Within the safe range, all indicators remain off to reflect normal operation.

To improve thermal response, a DC fan was integrated and is activated automatically when the temperature rises above the high limit. Because the LCD, fan, and other peripherals draw significant current, the system is powered using an external 8V battery with a voltage regulator that supplies a stable 5V to both the Arduino and the fan. This ensures reliable performance and avoids overloading the Arduino’s onboard regulator. Overall, the project demonstrates core embedded-system concepts such as sensor interfacing, digital input handling, peripheral control, and proper power-distribution design.

### Purpose

The purpose of this project is to design and implement a reliable embedded system capable of monitoring ambient temperature and responding to changes through visual, audible, and mechanical indicators. The system allows users to define adjustable temperature limits and provides immediate feedback when the measured temperature deviates from these thresholds. By incorporating a DC fan, LEDs, a buzzer, and an LCD interface, the project aims to demonstrate practical temperature-control mechanisms commonly found in industrial, home-automation, and safety-monitoring applications. Additionally, the project highlights the importance of proper power management and modular embedded-system design.

### Scope

The scope of this project includes the hardware and software development of a microcontroller-based temperature control system using an Arduino Mega, LM35 temperature sensor, 16×2 LCD, four push buttons, indicator LEDs, a buzzer, and a cooling fan. The system supports real-time temperature measurement, user-configurable high and low limits, and automatic activation of alarms and cooling mechanisms. This project focuses on sensor interfacing, digital input processing, peripheral control, display management, and regulated power distribution. The scope is limited to single-sensor temperature monitoring and does not include data logging, wireless communication, or advanced PID temperature regulation, though the current design can be extended to support such features in future iterations.

---

## Architectural Overview

@startuml
title Temperature Monitoring & Control System - Architecture Overview

rectangle "System Architecture" #FFE5CC {

  ' =======================
  ' High-Level Layer (Application)
  ' =======================
  rectangle "High-Level Layer\n(Application Layer)" #FFCC99 {
      rectangle "Application Logic\n(app.ino)"
  }

  ' =======================
  ' Middle-Level Layer (HAL)
  ' =======================
  rectangle "Middle-Level Layer\n(HAL - Hardware Abstraction Layer)" #CCFFCC {
      rectangle "Fan Driver\n(Fan.h / Fan.ino)"
      rectangle "LCD Driver\n(LCD.h / LCD.ino)"
      rectangle "LED Driver\n(LED.h / LED.ino)"
      rectangle "LM35 Sensor Driver\n(LM35.h / LM35.ino)"
      rectangle "Buttons Driver\n(Buttons.h / Buttons.ino)"
      rectangle "Display\n(Display.h / Display.ino)"
  }

  ' =======================
  ' Low-Level Layer (Common Utilities)
  ' =======================
  rectangle "Low-Level Layer\n(Common Utilities)" #FFF2CC {
      rectangle "Bitwise Macros\n(macros.h)"
      rectangle "Standard Types\n(std_types.h)"
  }

  ' =======================
  ' Physical Layer (Hardware)
  ' =======================
  rectangle "Physical Layer\n(Hardware Components)" #E6E6E6 {
      rectangle "Arduino Board / MCU"
      rectangle "LM35 Temperature Sensor"
      rectangle "Push Buttons"
      rectangle "Fan"
      rectangle "LCD"
      rectangle "LED"
  }


  ' =======================
  ' Relationships Between Layers
  ' =======================
  "Application Logic\n(app.ino)" -down-> "Fan Driver\n(Fan.h / Fan.ino)"
  "Application Logic\n(app.ino)" -down-> "LCD Driver\n(LCD.h / LCD.ino)"
  "Application Logic\n(app.ino)" -down-> "LED Driver\n(LED.h / LED.ino)"
  "Application Logic\n(app.ino)" -down-> "LM35 Sensor Driver\n(LM35.h / LM35.ino)"
  "Application Logic\n(app.ino)" -down-> "Buttons Driver\n(Buttons.h / Buttons.ino)"
  "Application Logic\n(app.ino)" -down-> "Display\n(Display.h / Display.ino)"

  "Fan Driver\n(Fan.h / Fan.ino)" -down-> "Bitwise Macros\n(macros.h)"
  "LCD Driver\n(LCD.h / LCD.ino)" -down-> "Bitwise Macros\n(macros.h)"
  "LED Driver\n(LED.h / LED.ino)" -down-> "Bitwise Macros\n(macros.h)"
  "LM35 Sensor Driver\n(LM35.h / LM35.ino)" -down-> "Standard Types\n(std_types.h)"
  "Buttons Driver\n(Buttons.h / Buttons.ino)" -down-> "Standard Types\n(std_types.h)"

  "Bitwise Macros\n(macros.h)" -down-> "Arduino Board / MCU"
  "Standard Types\n(std_types.h)" -down-> "Arduino Board / MCU"

  "Arduino Board / MCU" -down-> "LM35 Temperature Sensor"
  "Arduino Board / MCU" -down-> "Push Buttons"
  "Arduino Board / MCU" -down-> "Fan"
  "Arduino Board / MCU" -down-> "LCD"
  "Arduino Board / MCU" -down-> "LED"
}

@enduml
---

## Assumptions and Constraints

### Assumptions

1. **Stable 5V regulated power supply**  
   It is assumed that the external voltage regulator provides a stable 5V output from the 8V battery, supplying the Arduino Mega, LCD, fan, LEDs, and sensors without noticeable voltage drops.

2. **Common ground for all components**  
   All modules (LCD, LM35, LEDs, fan, buttons) share the same ground reference with the Arduino to ensure accurate ADC readings and reliable digital logic operation.

3. **Proper sensor placement**  
   The LM35 temperature sensor is assumed to be positioned away from heat sources such as the voltage regulator, fan motor, or Arduino board to accurately measure ambient temperature.

4. **Normal user button interaction**  
   Buttons are assumed to be pressed momentarily and not excessively fast. Software debouncing is presumed sufficient to handle typical mechanical bouncing.

5. **Fan responds predictably to digital control**  
   The DC fan is expected to start reliably when PB4 goes HIGH and stop when PB4 goes LOW, given that its power is independently regulated.

6. **Realistic temperature limits**  
   Users are expected to configure the high and low limits within reasonable ranges (0°C–100°C), avoiding unrealistic or conflicting inputs.

7. **Operational LCD display**  
   The LCD’s contrast and backlight are assumed to be adjusted properly so temperature values and limit settings are always readable.

---

### Constraints

1. **Arduino I/O current limitations**  
   The Arduino Mega cannot directly power high-current devices like fans. The fan must rely on an external regulated supply and be controlled only through PB4.

2. **Single-threaded system**  
   The program relies on polling and fixed delays. Very fast temperature changes or rapid button presses may not be captured immediately.

3. **Limited LCD space**  
   The 16×2 character LCD restricts the amount of information displayed. Only the temperature and limit values can be shown at once.

4. **No advanced temperature filtering**  
   LM35 readings are displayed directly without smoothing or filtering, making the system vulnerable to noise or sudden airflow changes.

5. **Sampling interval constraint**  
   Temperature updates occur every 2 seconds. Events happening between updates will not be detected until the next cycle.

6. **Simple button interface**  
   Interaction is limited to four push buttons. There is no extended menu, encoder, or touchscreen interface for more complex settings.

7. **Possible rapid toggling near thresholds**  
   Without hysteresis, temperatures hovering around HL or LL may cause LEDs or the fan to toggle frequently.

8. **Battery dependency**  
   Since the system is powered by an external 8V battery, performance may be affected as the battery voltage drops, despite regulation.



#### Main Loop Behavior
 
@startuml
title Main Loop Activity (Coursework 2 Temperature Control Project)

start

:Check Timer;

if (Two Seconds Passed?) then (Yes)
  :Read Temperature;
  :Update Temperature Display;

  if (Temp >= High Limit?) then (Yes)
    :Turn ON High LED;
    :Turn OFF Low LED;
    :Fan ON;
  else (No)
    if (Temp <= Low Limit?) then (Yes)
      :Turn OFF High LED;
      :Turn ON Low LED;
      :Fan OFF;
    else (No)
      :Turn OFF Both LEDs;
      :Fan OFF;
    endif
  endif

  :Reset Timer;
else (No)
  :Skip Temperature Update;
endif

:Read Buttons;

if (High Limit Increase Pressed?) then (Yes)
  :Increase High Limit;
  :Update Limits Display;
endif

if (High Limit Decrease Pressed?) then (Yes)
  :Decrease High Limit;
  :Update Limits Display;
endif

if (Low Limit Increase Pressed?) then (Yes)
  :Increase Low Limit;
  :Update Limits Display;
endif

if (Low Limit Decrease Pressed?) then (Yes)
  :Decrease Low Limit;
  :Update Limits Display;
endif

:Delay 50 ms;

repeat
repeat while (true)

@enduml

---

## Functional Description

This chapter explains the complete software functionality of the Temperature Monitoring and Control System. It describes how each module operates, how the system responds to temperature changes, and how the user interacts with the device through buttons, the LCD, and the built-in indicators.

---

### 1. System Overview

The system continuously measures ambient temperature using an LM35 analog temperature sensor and displays the result on a 16×2 LCD. The user is able to configure two boundaries:

- **High Limit (HL)** – the upper temperature threshold  
- **Low Limit (LL)** – the lower temperature threshold  

The system uses four push buttons to adjust these limits:
- HL Increase  
- HL Decrease  
- LL Increase  
- LL Decrease  

Based on the measured temperature:

| Condition | System Response |
|----------|------------------|
| **Temp ≥ HL** | Red LED ON, Buzzer ON, Fan ON, Blue LED OFF |
| **Temp ≤ LL** | Blue LED ON, Red LED OFF, Fan OFF, Buzzer OFF |
| **LL < Temp < HL** | All indicators OFF (Normal zone) |

LCD Display Format:

- **Line 1:** `Temp: XX.X C`  
- **Line 2:** `HL:XX LL:YY`

Temperature is updated **every 2 seconds**, while button reading occurs every **50 ms**.

---

### 2. Temperature Sensing (LM35 Module)

The LM35 sensor outputs a voltage proportional to temperature (10 mV/°C).  
The LM35 driver is implemented in `LM35.h` and `LM35.ino` and includes:

#### **LM35_Init()**
- Sets AVcc (5 V) as the ADC reference  
- Enables the ADC and sets prescaler for stable sampling  

#### **LM35_Read()**
Steps executed during every reading:
1. Select ADC channel 0 (PA0 / A0)
2. Start analog-to-digital conversion
3. Wait until conversion finishes
4. Read the raw ADC value (0–1023)
5. Convert to voltage:  
   `voltage = (ADC_value * 5.0) / 1024.0`
6. Convert to temperature:  
   `temperatureC = voltage * 100.0`

The result is saved to `last_temp`.

---

### 3. LCD & Display Module (LCD + Display helpers)

The LCD works in **4-bit mode** and uses PORTA for data lines and PORTH for control signals (RS, EN).

Main LCD API functions:
- `LCD_Init()` – initialize LCD hardware  
- `LCD_Command()` – send control commands  
- `LCD_Char()` – write a single character  
- `LCD_String()` – write an entire string  
- `LCD_String_xy()` – print at a specific row/column  
- `LCD_ClearLine(line)` – clear a single LCD line  

Two higher-level UI update functions exist in `Display.h/.ino`:

#### **update_temp_display()**
- Clears line 1  
- Prints formatted temperature with one decimal place  

#### **update_limits_display()**
- Clears line 2  
- Displays new HL and LL values  

---

### 4. Button Input & Limit Adjustment (Buttons Module)

The system includes **four buttons**, each mapped to one function:

| Button Pin | Function |
|------------|----------|
| PC0 | High Limit Increase |
| PC1 | High Limit Decrease |
| PC2 | Low Limit Increase |
| PC3 | Low Limit Decrease |

The button driver performs:
- Input pin configuration  
- Internal pull-up activation (pin reads HIGH when not pressed)
- Digital reading logic (button pressed = LOW)

#### Button Behavior in Software
Each button triggers:
- Limit adjustment (+1°C or –1°C)
- Safety correction:
  - LL must always be **< HL**
  - LL ≥ 0  
  - HL ≤ 100  
- LCD limit display refresh  
- 200 ms debounce delay  

---

### 5. LED, Buzzer, and Fan Control

The system uses:

| Component | Pin | Purpose |
|----------|-----|---------|
| Red LED | PC4 | High temperature indicator |
| Blue LED | PC5 | Low temperature indicator |
| Fan | PB4 | Active cooling above HL |
| Buzzer | (optional) | Alarm above HL |

All outputs are digital and controlled by:

- `LED_Init()`  
- `LED_On_High()` / `LED_Off_High()`  
- `LED_On_Low()` / `LED_Off_Low()`  
- `Fan_Init()`  
- `Fan_On()` / `Fan_Off()`  

#### Control Logic
- If `last_temp >= high_limit`: red LED + buzzer + fan ON  
- If `last_temp <= low_limit`: blue LED ON, fan OFF  
- If between limits: all indicators OFF  

The fan receives power from an **external regulated 5V** source due to high current draw.

---

### 6. Program Flow

The system follows a simple **polling-based control loop**:

#### At startup:
1. Initialize LCD, LEDs, buttons, LM35, and fan  
2. Display initial temperature and thresholds  

#### Main loop tasks:
1. **Timer Update:**  
   Add 50 ms each iteration  

2. **Every 2 seconds:**  
   - Read LM35 temperature  
   - Update LCD temperature line  
   - Apply control logic (LEDs, fan, buzzer)  

3. **Every 50 ms:**  
   - Check all four buttons  
   - Update HL and LL if pressed  
   - Update LCD line 2  

4. **Loop delay:**  
   `_delay_ms(50);`

---

### 7. System Behavior Summary

| Condition | System Action | Indicators |
|-----------|---------------|------------|
| Temp ≥ HL | Fan ON, Alarm ON | Red LED ON |
| Temp ≤ LL | Fan OFF | Blue LED ON |
| LL < Temp < HL | Normal operation | All LEDs OFF |
| HL Increase Button | HL = HL + 1 | LCD updated |
| HL Decrease Button | HL = HL – 1 | LCD updated |
| LL Increase Button | LL = LL + 1 | LCD updated |
| LL Decrease Button | LL = LL – 1 | LCD updated |

---


### 1. System Overview

The system continuously measures ambient temperature using an LM35 analog sensor and displays the current reading on a 16×2 LCD. It allows the user to configure two temperature boundaries:

- **High Limit (HL):** Upper threshold  
- **Low Limit (LL):** Lower threshold  

Based on the measured temperature, the system activates output indicators:
- **Blue LED** → Temperature below the low limit  
- **Red LED + Fan + Buzzer (if enabled)** → Temperature above the high limit  
- **All indicators OFF** → Temperature within the limits  

The display shows:
- Line 1: `Temp: XX.X C`  
- Line 2: `HL:XX LL:YY`

### 2. Initialization Phase

At system startup, the following modules are initialized:

- **LCD Driver:** Configures PORTA and PORTH for 4-bit communication  
- **LED Driver:** Sets PC4 and PC5 as digital outputs  
- **Fan Driver:** Configures PB4/PD6 (according to your wiring) as output  
- **Buttons Driver:** Enables PC0–PC3 as digital inputs with internal pull-ups  
- **LM35 Driver:** Configures ADC with AVcc reference and prescaler  

The display is updated once with the initial temperature and limits.

### 3. Temperature Measurement (LM35 & ADC Module)

The LM35 sensor outputs a voltage proportional to temperature (10 mV/°C).  
The ADC converts this voltage into a digital value:


Temperature is sampled every **2 seconds** using a software timer.  
The measured value is stored in the global variable:


### 4. Display Management (LCD & Display Module)

The display module provides helper functions:

- `update_temp_display()` – Clears line 1 and prints formatted temperature  
- `update_limits_display()` – Clears line 2 and prints HL/LL values  

The LCD always reflects the most recent temperature and user set limits.

### 5. Button Handling (Buttons Module)

The system includes **four push buttons**:

| Button | Function |
|--------|----------|
| High Limit Increase  | Raise HL by 1°C |
| High Limit Decrease  | Lower HL by 1°C |
| Low Limit Increase   | Raise LL by 1°C |
| Low Limit Decrease   | Lower LL by 1°C |

Software behavior includes:

- Internal pull-ups → Buttons read **LOW when pressed**
- 200 ms debounce after each press
- Safety constraints:
  - `HL <= 100°C`
  - `LL >= 0°C`
  - `LL` is always at least **1°C below** `HL`

After each adjustment, line 2 of the LCD is updated.

### 6. Temperature-Control Logic

After each temperature update, the control logic determines the system state:

#### **Case 1: Temperature ≥ High Limit**
- High LED ON  
- Low LED OFF  
- Fan ON  
- Buzzer (if used) ON  

#### **Case 2: Temperature ≤ Low Limit**
- High LED OFF  
- Low LED ON  
- Fan OFF  
- Buzzer OFF  

#### **Case 3: Temperature Within Limits**
- All LEDs OFF  
- Fan OFF  
- Buzzer OFF  

This ensures clear visual and mechanical feedback based on thermal conditions.

### 7. Fan Control Logic (Fan Module)

The fan is controlled by a digital output pin (PD6 or PB4 depending on wiring):

- `Fan_On()` sets the output HIGH  
- `Fan_Off()` sets it LOW  

The fan operates **only** during high-temperature conditions.

### 8. Main Loop Operation

The main loop performs the following tasks repeatedly:

1. Update internal timer  
2. Every 2 seconds:
   - Read temperature from LM35  
   - Update the LCD  
   - Update LEDs and fan based on thresholds  
3. Read button states  
4. Apply debouncing and limit logic  
5. Update HL and LL display if needed  
6. Delay for 50 ms (loop timing)

This creates a simple **polling-based embedded control loop**.

### 9. Error Handling and System Limits

The system includes several protections:

- HL cannot exceed 100°C  
- LL cannot drop below 0°C  
- LL is always strictly less than HL  
- Fan draws power from an external regulator to avoid overloading Arduino  
- LCD line clearing prevents ghost characters  
- Buttons include software debounce to avoid accidental multi-presses  

### 10. Summary of Functional Behavior

The software coordinates sensing, display, user input, and actuator control.  
Overall, it achieves the following:

- Reliable real-time temperature monitoring  
- Immediate response to unsafe temperature conditions  
- Smooth user control of adjustable limits  
- Clear and organized firmware structure using modular drivers  

---
## Implementation of the Module

### Hardware Implementation

This chapter explains how the components are connected around the Arduino Mega board.

### 1. Components Used

| **Component**              | **Description**                                                | **Quantity** |
| -------------------------- | -------------------------------------------------------------- | ------------ |
| Arduino Mega 2560          | Main microcontroller board used to run the embedded firmware. | 1            |
| LM35 sensor                | Analog temperature sensor (10 mV/°C) for ambient measurement. | 1            |
| 16×2 LCD                   | Character display used to show current temperature and limits.| 1            |
| DC Fan                     | Cooling actuator that turns on when temperature exceeds HL.   | 1            |
| NPN transistor / MOSFET    | Driver to switch the fan using a digital pin (e.g. PB4).      | 1            |
| Flyback diode              | Protects the transistor from inductive spikes from the fan.   | 1            |
| LED (High / Red)           | Indicates temperature ≥ high limit (over-temperature).        | 1            |
| LED (Low / Blue)           | Indicates temperature ≤ low limit (under-temperature).        | 1            |
| Buzzer                     | Audible alarm when the temperature exceeds the high limit.    | 1            |
| Push Buttons (HL+/HL−/LL+/LL−) | Four buttons to increase/decrease high and low limits.   | 4            |
| 8V Battery (or supply)     | External power source for the system.                         | 1            |
| 5V Voltage regulator       | Provides stable 5V for Arduino, fan driver, LCD, and logic.   | 1            |
| Potentiometer              | Adjusts LCD contrast.                                          | 1            |
| Resistors                  | Current limiting for LEDs, transistor base, etc.              | Several      |
| Breadboard                 | Prototyping platform for wiring the circuit.                  | 1            |
| Jumper wires               | Used to connect all components together.                      | Several      |

### 2. Circuit Description

* **Power Supply (8V Battery & Voltage Regulator):**
  * The system is powered using an external 8V battery.
  * A 5V voltage regulator provides stable power to:
    - Arduino Mega
    - LCD
    - LM35 sensor
    - LEDs
    - Buttons
    - Fan driver circuit
  * All components share a **common ground**.

* **LM35 Temperature Sensor:**
  * Vcc connected to +5V (from regulator).
  * GND connected to system ground.
  * Output connected to **ADC0** (`PA0` / `A0`) for analog reading.

* **16×2 LCD (4-bit Mode):**
  * Data lines connected to the **upper 4 bits of PORTA**.
  * RS and EN control pins connected to **PORTH** (`PH5` and `PH6`).
  * LCD is powered from the regulated 5V supply.
  * A potentiometer is used to control the contrast.

* **High LED (Red):**
  * Connected to **PC4** with a current-limiting resistor.
  * Active when the measured temperature ≥ high limit.

* **Low LED (Blue):**
  * Connected to **PC5** with a current-limiting resistor.
  * Active when the measured temperature ≤ low limit.

* **Push Buttons (4 Buttons):**
  * **HL+ (Increase High Limit)** → connected to **PC0**.
  * **HL− (Decrease High Limit)** → connected to **PC1**.
  * **LL+ (Increase Low Limit)** → connected to **PC2**.
  * **LL− (Decrease Low Limit)** → connected to **PC3**.
  * All buttons use internal pull-up resistors:
    - Logic HIGH when unpressed
    - Logic LOW when pressed

* **Fan & Driver Circuit:**
  * Fan powered directly from the **regulated 5V** output (not Arduino).
  * Transistor or MOSFET used for switching.
  * Control signal connected to **PB4**.
  * A flyback diode is used across the fan to protect against inductive spikes.
  * Fan activates when temperature ≥ high limit.

* **Buzzer:**
  * Connected to a digital output pin (according to your actual wiring).
  * Activated together with the High LED when temperature exceeds the high limit.

---

### 3. Circuit Connections Summary

| **Component**     | **Arduino Pin**     | **Type**                          |
|-------------------|----------------------|-----------------------------------|
| LM35 Output       | PA0 / A0 (ADC0)      | Analog input                      |
| LCD Data Lines    | PORTA (upper nibble) | 4-bit data output                 |
| LCD RS            | PH5                  | Control output                    |
| LCD EN            | PH6                  | Control output                    |
| High LED (Red)    | PC4                  | Digital output                    |
| Low LED (Blue)    | PC5                  | Digital output                    |
| HL+ Button        | PC0                  | Digital input (pull-up)           |
| HL− Button        | PC1                  | Digital input (pull-up)           |
| LL+ Button        | PC2                  | Digital input (pull-up)           |
| LL− Button        | PC3                  | Digital input (pull-up)           |
| Fan Control       | PB4                  | Digital output (to transistor)    |
| Buzzer            | (Your chosen pin)    | Digital output                    |
| Common Ground     | GND                  | Reference for all components      |
| Power Input       | 8V Battery → Reg 5V  | System power source               |

---

### 4. Working Principle

* The **LM35 sensor** outputs a voltage proportional to the ambient temperature.
* The analog signal is read by the ADC and converted to a temperature value in °C.
* The current temperature is displayed on the LCD and compared with the **High Limit (HL)** and **Low Limit (LL)**.
  
* **User Interaction:**
  * Four buttons allow the user to adjust HL and LL values:
    - HL+ increases high limit  
    - HL− decreases high limit  
    - LL+ increases low limit  
    - LL− decreases low limit  
  * The updated limits are shown on the LCD.

* **System Response:**
  - **If temperature ≥ HL:**  
    - High LED turns ON  
    - Low LED turns OFF  
    - Fan turns ON  
    - Buzzer activates  
  - **If temperature ≤ LL:**  
    - High LED turns OFF  
    - Low LED turns ON  
    - Fan turns OFF  
    - Buzzer OFF  
  - **If LL < temperature < HL:**  
    - Both LEDs OFF  
    - Fan OFF  
    - Buzzer OFF

* The system continuously repeats this cycle with periodic temperature updates and responsive button handling.

---


## Integration and Configuration

### Static Files

| **File name**                 | **Contents**                                                                                                                                      |
| ----------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------- |
| `app.ino`                     | Main application file. Initializes all modules and implements the main loop: temperature sampling, limit management, LCD updates, and LED/fan logic. |
| `LCD.ino` / `LCD.h`           | LCD driver in 4-bit mode. Handles initialization, commands, character/ string printing, cursor positioning, and single-line clearing.            |
| `LED.ino` / `LED.h`           | Driver for the status LEDs. Configures LED pins as outputs and provides functions to turn the High (red) and Low (blue) LEDs ON and OFF.        |
| `LM35.ino` / `LM35.h`         | Temperature sensor driver. Configures the ADC and reads the LM35 analog value, converting it into a temperature value in °C.                     |
| `Buttons.ino` / `Buttons.h`   | Button driver. Initializes the four push buttons with internal pull-ups and provides functions to detect HL+/HL−/LL+/LL− button presses.        |
| `Fan.ino` / `Fan.h`           | Fan driver. Configures the fan control pin and provides simple `Fan_On()` / `Fan_Off()` functions for driving the external transistor/MOSFET.   |
| `Display.ino` / `Display.h`   | UI helper module. Implements high-level functions such as `update_temp_display()` and `update_limits_display()` to format and refresh the LCD.   |
| `macros.h`                    | Common bitwise macros (SET_BIT, CLEAR_BIT, TOGGLE_BIT, BIT_IS_SET, etc.) used for low-level register manipulation and cleaner I/O code.         |
| `std_types.h`                 | Standard type definitions (e.g., `uint8`, `uint16`, `sint8`, etc.) to improve portability and readability of the embedded C code.                |

---
## Include Structure

@startuml
title Coursework 2 - Temperature Monitoring & Control Project\nFile Dependency Overview

package "Source Code Structure" {

    [app.ino] --> [LCD.h]
    [app.ino] --> [LED.h]
    [app.ino] --> [LM35.h]
    [app.ino] --> [Buttons.h]
    [app.ino] --> [Fan.h]
    [app.ino] --> [Display.h]
    [app.ino] --> [std_types.h]
    [app.ino] --> [macros.h]

    [LCD.ino] --> [LCD.h]
    [LCD.ino] --> [std_types.h]
    [LCD.ino] --> [macros.h]

    [LED.ino] --> [LED.h]
    [LED.ino] --> [std_types.h]
    [LED.ino] --> [macros.h]

    [LM35.ino] --> [LM35.h]
    [LM35.ino] --> [std_types.h]

    [Buttons.ino] --> [Buttons.h]
    [Buttons.ino] --> [std_types.h]
    [Buttons.ino] --> [macros.h]

    [Fan.ino] --> [Fan.h]
    [Fan.ino] --> [std_types.h]
    [Fan.ino] --> [macros.h]

    [Display.ino] --> [Display.h]
    [Display.ino] --> [LCD.h]
    [Display.ino] --> [std_types.h]
}
@enduml
---

### Configuration Parameters

| **Name**           | **Default Value** | **Description**                                                                 |
|--------------------|-------------------|---------------------------------------------------------------------------------|
| `high_limit`       | 30.0 °C           | Initial high-temperature threshold shown on the LCD.                           |
| `low_limit`        | 20.0 °C           | Initial low-temperature threshold shown on the LCD.                            |
| `TEMP_INTERVAL`    | 2000 ms           | Interval between temperature updates (LM35 read + LCD refresh every 2 seconds). |
| `LOOP_DELAY`       | 50 ms             | Main-loop delay controlling button polling and program execution rate.          |
| `DEBOUNCE_DELAY`   | 200 ms            | Debounce delay applied after every button press.                                |
| `HL_MAX`           | 100 °C            | Maximum permissible high-temperature limit.                                     |
| `LL_MIN`           | 0 °C              | Minimum permissible low-temperature limit.                                      |
| `HL_LL_GAP`        | 1 °C              | Mandatory gap ensuring `low_limit` is always at least 1°C below `high_limit`.  |
| `LM35_ADC_CHANNEL` | ADC0 (PA0)        | Analog channel used for LM35 temperature sensing.                               |
| `LCD_DATA_PINS`    | PORTA (A4–A7)     | LCD 4-bit data lines connected to the upper nibble of PORTA.                    |
| `LCD_RS_PIN`       | PH5               | LCD Register Select (RS) control pin.                                           |
| `LCD_EN_PIN`       | PH6               | LCD Enable (EN) control pin.                                                    |
| `LED_HIGH_PIN`     | PC4               | Output pin driving the High-temperature indicator LED.                          |
| `LED_LOW_PIN`      | PC5               | Output pin driving the Low-temperature indicator LED.                           |
| `BUTTON_HL_INC`    | PC0               | Push button to increase the High Limit (internal pull-up).                      |
| `BUTTON_HL_DEC`    | PC1               | Push button to decrease the High Limit (internal pull-up).                      |
| `BUTTON_LL_INC`    | PC2               | Push button to increase the Low Limit (internal pull-up).                       |
| `BUTTON_LL_DEC`    | PC3               | Push button to decrease the Low Limit (internal pull-up).                       |
| `FAN_PIN`          | PD6               | Digital output pin controlling the cooling fan via transistor/MOSFET.           |
| `POWER_SOURCE`     | 8V Battery + 5V Regulator | External supply powering both the Arduino and the fan through a regulator. |

---


## Conclusion

This project brought together multiple embedded-system concepts to create a reliable and responsive temperature-monitoring and control system. Using an Arduino Mega, an LM35 temperature sensor, a 16×2 LCD, LEDs, four push buttons, and a fan, the system was designed to sense the environment, display real-time data, and react automatically whenever the temperature moved outside the user-defined limits.

One of the key successes of this project was achieving smooth interaction between all the hardware modules. The temperature readings were captured accurately through the ADC, the LCD provided clear and continuous feedback, and the push buttons allowed the user to adjust limits easily. The LEDs and fan behaved exactly as intended, providing immediate visual and mechanical responses based on the measured temperature. Integrating an external 8V battery with a 5V regulator also ensured that the system remained stable, especially when powering current-demanding components like the LCD and fan.

The modular design of the code — separating drivers for the LCD, LM35, buttons, LEDs, and fan — made the system easier to understand, maintain, and extend. This structure reflects good engineering practice and prepares the project for future enhancements such as data logging, wireless monitoring, or more advanced temperature-control logic.

Overall, the project successfully met its objectives and provided valuable hands-on experience in both hardware integration and embedded software development. It not only demonstrated how individual components work on their own but also how they can come together to form a cohesive, dependable, and practical embedded system.

