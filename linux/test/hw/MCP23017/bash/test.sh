#!/bin/bash
# Author: HardElettroSoft
echo
echo "Test for MCP23017"


#MCP23017 at reset:
  #pin ALL INPUT
#READ GPIOA
echo "GPIOA value:"
GPIOA=$(i2cget -y 0 0x27 0x12 2>&1)
echo $GPIOA

echo "Rotary selector on GPIOB:"
GPIOB=$(i2cget -y 0 0x27 0x13 2>&1)
SELECTOR=$(( 15 - ( GPIOB & 0x0F ) ))
echo $SELECTOR


#config pins with led on gpiob in output mode
i2cset -y 0 0x27 0x01 0x0F
echo "Only D1 on"
i2cset -y 0 0x27 0x13 0x10
sleep 1
echo "Only D2 on"
i2cset -y 0 0x27 0x13 0x20
sleep 1
echo "Only D3 on"
i2cset -y 0 0x27 0x13 0x40
sleep 1
echo "Only D4 on"
i2cset -y 0 0x27 0x13 0x80
sleep 1

echo "All led off"
i2cset -y 0 0x27 0x13 0x00

echo "End"
echo


