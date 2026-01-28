# Plate Sensor Project

Detects when a shooting plate falls and sends an RF signal.

## Components

- ESP32-C3 (XIAO_ESP32C3)
- SC7A20H accelerometer (I2C)
- RF transmitter
- 3.3V battery

## Pin Configuration

| Component | ESP32-C3 Pin |
|-----------|--------------|
| Accelerometer SDA | GPIO 8 |
| Accelerometer SCL | GPIO 9 |
| RF Transmitter | GPIO 2 |

## How It Works

1. The accelerometer monitors the orientation of the plate
2. When the plate is "down" (z-axis near 0g), the device sends an RF signal
3. Uses deep sleep between readings to conserve battery

## Libraries Required

- Wire.h (built-in)
- RF transmitter library (RCSwitch or similar)

## Building

```bash
cd ~/arduino-projects
./scripts/build.sh esp32/projects/plate_sensor
```

## Uploading

```bash
./scripts/upload.sh esp32/projects/plate_sensor /dev/ttyUSB0
```
