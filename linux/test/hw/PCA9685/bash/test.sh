#!/bin/bash
# Author: HardElettroSoft

echo "Test for PCA9685"

#PCA9685 at reset:
  #pin output as totem pole as default
  #all PWM off by default
  #sleep mode
#software reset NOT HANDLED BY I2CSET, CHIP ID MUST MUST BE > 0X03
#i2cset -y 0 0x00 0x06 ?
#GO TO SLEEP
i2cset -y 0 0x5f 0x00 0x11
sleep 0.1

#setup freq 200hz -> 50Hz
echo "init freq -> 50Hz = 20ms (0x79)"
i2cset -y 0 0x5f 0xFE 0x79

#POWER ON
i2cset -y 0 0x5f 0x00 0x00

echo "pwm ch3 to 50% 1.5ms"

#test for servomotor on pin LED3
#all led at center: 1,5ms su 20ms
#pwm3 1.5ms
#LED3_ON_L
i2cset -y 0 0x5f 0x12 0x85

echo "set initial value for servomotor"
#LED3_ON_H
i2cset -y 0 0x5f 0x13 0x03
#LED3_OFF_L       #51 TO FF    BF
i2cset -y 0 0x5f 0x14 0x51
#LED3_OFF_H
i2cset -y 0 0x5f 0x15 0x04
sleep 2

echo "start simple rotation test"
i2cset -y 0 0x5f 0x14 0xFF
sleep 2
i2cset -y 0 0x5f 0x14 0xBF
sleep 2
i2cset -y 0 0x5f 0x14 0x51
sleep 2

i2cset -y 0 0x5f 0x14 0x61
sleep 0.2
i2cset -y 0 0x5f 0x14 0x71
sleep 0.2
i2cset -y 0 0x5f 0x14 0x81
sleep 0.2
i2cset -y 0 0x5f 0x14 0x91
sleep 0.2
i2cset -y 0 0x5f 0x14 0xA1
sleep 0.2
i2cset -y 0 0x5f 0x14 0xB1
sleep 0.2
i2cset -y 0 0x5f 0x14 0xC1
sleep 0.2
i2cset -y 0 0x5f 0x14 0xD1
sleep 0.2
i2cset -y 0 0x5f 0x14 0xE1
sleep 0.2
i2cset -y 0 0x5f 0x14 0xF1
sleep 2

#INIT CONDITION
i2cset -y 0 0x5f 0x14 0x51

echo "FINISH"
