# Project instructions

I want to make a device that I can attach to the back of a metal plate, when I shoot the plate down, the device should sense that its down and send a signal through an rf transmitter. I will include a list of components im using below:

- esp32c3 (Specifically: XIAO_ESP32C3)
- sc7a20h accelerometer with 4 pins (vcc, gnd, rx, tx)
- standard rf transmitter
- A 3.3v battery

Im using pins 8 and 9 as rx and tx on the esp32 for sending/receiving to/from the accelerometer and pin 2 for activating the rf transmitter
