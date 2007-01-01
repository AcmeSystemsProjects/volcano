#!/bin/bash
# Author: HardElettroSoft
echo
echo "Test for detect all I2C chip"
echo

i2cdetect -y 0

echo
echo "0x27: MCP23017 - I/O expander with 16 gpio"
echo "0x48: PCF8591  - ADC + DAC 8 bit"
echo "0x4B: ADS7828E - ADC 12 bit"
echo "0x50: ST24C16  - EEPROM 2Kbit x8 bit, Block 0"
echo "0x51: ST24C16  - EEPROM 2Kbit x8 bit, Block 1"
echo "0x52: ST24C16  - EEPROM 2Kbit x8 bit, Block 2"
echo "0x53: ST24C16  - EEPROM 2Kbit x8 bit, Block 3"
echo "0x54: ST24C16  - EEPROM 2Kbit x8 bit, Block 4"
echo "0x55: ST24C16  - EEPROM 2Kbit x8 bit, Block 5"
echo "0x56: ST24C16  - EEPROM 2Kbit x8 bit, Block 6"
echo "0x57: ST24C16  - EEPROM 2Kbit x8 bit, Block 7"
echo "0x5F: PCA9685  - 16ch 12bit pwm expander"
echo "0x70: PCA9685  - ('all call' reserved register)"
echo


