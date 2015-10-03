

all:	lineFollower

lineFollower:	main.o gpioFastLib.o 
		gcc main.o gpioFastLib.o -o lineFollower

main.o:	main.c
	gcc -c main.c

gpioFastLib.o: ../../../../lib/gpioFastLib.c 
	gcc -c ../../../../lib/gpioFastLib.c 

clean:
	rm *.o lineFollower
