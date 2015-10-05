#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"
#include "line.h"

extern int position;
extern int lastDir;

void initGpio(){
	if (init_memoryToIO()) {
                printf ("Error in init_memoryToIO() \n");
                exit(1);
        }
	setGpioinInput(S_R);
	setGpioinInput(S_C);
	setGpioinInput(S_L);
}

void initVariable(){
	position=POS_START;
	lastDir=SENS_CLEAN;
}

void init(){
	closePwmIfOpened();
	initGpio();
	initPwm();
	initVariable();
	sleep(2);
}

