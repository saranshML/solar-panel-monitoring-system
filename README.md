# IoT-Based Solar Panel Monitoring System

An ESP32-C3 based solar panel monitoring system designed to measure voltage, current, power, energy generation, temperature, and estimated savings in real time.

The system uses ADS1115 for high-resolution data acquisition, ACS758 Hall-effect current sensing, DS18B20 temperature monitoring, and Blynk IoT Cloud for remote visualization.

## Features

- Real-time voltage monitoring (0–50V)
- Current monitoring up to 50A
- Power calculation
- Energy generation tracking (Wh)
- Temperature monitoring
- Estimated cost savings calculation
- Local LCD display
- Cloud monitoring using Blynk
- Wi-Fi enabled remote access

## Hardware Used

- Seeed Studio XIAO ESP32-C3
- ADS1115 16-bit ADC
- ACS758 Hall Effect Current Sensor
- DS18B20 Temperature Sensor
- I2C LCD Display
- Voltage Divider Network
- Solar PV Module

## System Architecture

[Insert Block Diagram Here]

## Circuit Diagram

[Insert Circuit Diagram Here]

## Software Stack

- Arduino IDE
- ESP32 Board Package
- Adafruit ADS1X15 Library
- DallasTemperature Library
- OneWire Library
- LiquidCrystal_I2C Library
- Blynk IoT Platform

## Results

The system successfully measures:

- Voltage
- Current
- Temperature
- Instantaneous Power
- Generated Energy
- Financial Savings

All parameters are displayed locally and uploaded to the Blynk Cloud dashboard for remote monitoring.

## Future Improvements

- MPPT Integration
- Fault Detection
- Cloud Data Logging
- AI-based Performance Prediction
- Solar Irradiance Measurement

## Authors

Saransh Pandey  
B.Tech Electrical Engineering  
G.B. Pant University of Agriculture & Technology

## License

MIT License
