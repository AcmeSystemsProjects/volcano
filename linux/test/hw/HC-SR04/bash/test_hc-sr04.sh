#!/bin/bash
# Autor: HardElettroSoft

echo
echo "Program start - If this script not work, You need HC-SR04 module loaded for your Kernel"
echo
# contiene microsecondi
SENSOR=/sys/class/hcsr04/value

#Read microseconds
US=$(head -n 1 $SENSOR)

# calculate cm
CM=$(echo "$US / 58" |bc -l)

echo "$CM cm "
echo
echo
