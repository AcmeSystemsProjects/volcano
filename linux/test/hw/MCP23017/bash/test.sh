#!/bin/bash
# Author: HardElettroSoft
echo
echo "Test for MCP23017"

#MCP23017 at reset:
  #pin ALL INPUT
#READ GPIO
echo "buttons on GPIOB:"
GPIO=$(i2cget -y 0 0x27 0x13 2>&1)
echo $GPIO

#config gpioa in output mode
i2cset -y 0 0x27 0x00 0x00
#switch on backlight
echo "retroilluminazione accesa"
i2cset -y 0 0x27 0x12 0x02

echo "attesa 1 secondo"
sleep 1

echo "retroilluminazione spenta"
i2cset -y 0 0x27 0x12 0x00

echo "accendo un led per volta"
i2cset -y 0 0x27 0x12 0x01
sleep 0.5
i2cset -y 0 0x27 0x12 0x04
sleep 0.5
i2cset -y 0 0x27 0x12 0x08
sleep 0.5
i2cset -y 0 0x27 0x12 0x10
sleep 0.5
i2cset -y 0 0x27 0x12 0x20
sleep 0.5
i2cset -y 0 0x27 0x12 0x40
sleep 0.5
i2cset -y 0 0x27 0x12 0x80
sleep 0.5
i2cset -y 0 0x27 0x12 0x00
sleep 0.5
echo "fine"

echo


