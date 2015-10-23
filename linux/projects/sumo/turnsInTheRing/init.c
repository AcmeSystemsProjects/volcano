
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pins.h"


#define SCAN 0
#define CHECK 1
#define STOP 2

extern int prev;
extern int dir;
extern int dirOld;


void initVars(){
    prev=0;
    dir=FORWARD;
    dirOld=SENS_C;
}

void initGpio(){
    if (init_memoryToIO()) {
        printf ("Error in init_memoryToIO() \n");
        exit(1);
    }

    setGpioinInput(SENS_R);
    setGpioinInput(SENS_C);
    setGpioinInput(SENS_L);
}

void startupPwm(){
    closePwm();
    sleep(1);
    initPwm();
}

void init(){
    initVars();
    startupPwm();
    initGpio();
}



void selfTest(){
    indietro();
    sleep(2);
    ruotaDX();
    sleep(2);
    ruotaSX();
    sleep(2);
    pwmOff();
    sleep(1);
}
