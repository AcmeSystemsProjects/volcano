#!/bin/bash
# Author: HardElettroSoft
echo
echo "Test for ST24C16"

echo "Do you want read single register [r] or write [w] ? "
read  -n 1 OPERATION
echo
if ! [[ $OPERATION =~ ^(r|w|R|W)$ ]]; then
	echo "wronk choice"
	exit
fi


echo "Insert bank 0..7"
read -n 1 BANK
echo
declare -r BANK_VALIDATOR='^[0-7]+$'
if ! [[ $BANK =~ $BANK_VALIDATOR ]]; then
	echo "wrong value"
	exit
fi
#eeprom address calculation
ADDRESS=$((BANK + 0x50))


echo "insert register 0..255"
#http://stackoverflow.com/questions/1377926/regular-expression-numeric-range
declare -r BYTE_VALIDATOR='^^(0?[0-9]{1,2}|1[0-7][0-9]|255)$'
read REGISTER
if ! [[ $REGISTER =~ $BYTE_VALIDATOR ]]; then
        echo "wrong value"
        exit
fi

#read ?
if  [[ $OPERATION =~ ^(r|R)$ ]]; then
	i2cget -y 0 $ADDRESS $REGISTER

        echo
        exit
fi


#write
if  [[ $OPERATION =~ ^(w|W)$ ]]; then
	echo "insert value 0..255"
	read VALUE
	if ! [[ $VALUE =~ $BYTE_VALIDATOR ]]; then
        	echo "wrong value"
        	exit
	fi

	i2cset -y 0 $ADDRESS $REGISTER $VALUE

fi


echo
echo


