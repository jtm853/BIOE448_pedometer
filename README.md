# Juicer Pedometer

**BIOE 448 Final Project**  
Created by: Jonathan Makhoul and Alice Tian  
Last modified: 4/15/25

## Overview

**Juicer Pedometer** is a compact, proof-of-concept pedometer built using an accelerometer, a microcontroller, and Bluetooth Low Energy (BLE) capabilities. The device detects and counts user steps in real-time, then uses this data to estimate related parameters such as **calories burned** and **distance traveled**. These values are displayed on a local **16x2 LCD screen** and can also be transmitted wirelessly via Bluetooth for real-time remote access.

## Features

- Real-time **step counting** using acceleration magnitude thresholding
- Estimation of:
  - **Calories burned** (0.04 kcal per step)
  - **Distance traveled** (0.71 meters per step)
- **Bluetooth Low Energy (BLE)** transmission of step, calorie, and distance data
- **User interface** with a 16x2 LCD screen
- **Button-controlled scrolling** between calorie and distance displays
- Designed for **compact, wearable use**

## Hardware Requirements

- Arduino-compatible microcontroller with BLE (e.g., Arduino Nano 33 BLE Sense)
- ADXL345 accelerometer (or equivalent, I2C-based)
- 16x2 LCD display (using 6 digital pins)
- Push button (for toggling display)
- Power source (battery or USB)
- Optional: LED indicator for BLE connection

## Libraries Used

- `Wire.h` — for I2C communication with the accelerometer
- `LiquidCrystal.h` — for interfacing with the 16x2 LCD
- `ArduinoBLE.h` — for BLE service and characteristic communication

## Bluetooth Characteristics

| Characteristic       | UUID   | Description          |
|----------------------|--------|----------------------|
| `read_StepCount`     | 2A57   | Number of steps taken |
| `read_CalBurned`     | 2A58   | Estimated kcal burned |
| `read_Distance`      | 2A59   | Estimated meters walked |

Bluetooth service UUID: `180A`  
Local advertised name: **"Juicer Pedometer"**

## How It Works

1. The ADXL345 accelerometer captures raw x, y, z acceleration data.
2. The magnitude of the acceleration vector is computed.
3. If this magnitude exceeds a threshold (set to 450) and no peak was previously detected, a step is counted.
4. The step count is used to calculate calories burned and distance traveled.
5. Values are:
   - Displayed on the LCD screen
   - Sent via BLE characteristics to a connected central device

## File List

- `pedometer_main.ino` — Full Arduino sketch containing step detection, Bluetooth setup, LCD display logic, and user interface

## Acknowledgements

This project was completed as part of the **BIOE 448: Senior Design** course at Rice University.

---
