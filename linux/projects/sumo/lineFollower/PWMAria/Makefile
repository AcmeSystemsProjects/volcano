

all:	lineFollower

lineFollower:	main.o gpioFastLib.o sumoMotor.o init.o line.o
		gcc main.o gpioFastLib.o sumoMotor.o init.o line.o -o lineFollower

main.o:	main.c gpio.h line.h
	gcc -c main.c

gpioFastLib.o: ../../../../lib/gpio/gpioFastLib.c 
	gcc -c ../../../../lib/gpio/gpioFastLib.c 

sumoMotor.o: sumoMotor.c
	gcc -c sumoMotor.c

init.o: init.c gpio.h line.h
	gcc -c init.c

line.o: line.c line.h
	gcc -c line.c
clean:
	rm *.o lineFollower
