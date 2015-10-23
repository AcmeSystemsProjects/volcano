#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pins.h"

#define BLACK 1

void printBanner(){
	printf("\n\n\rPROGRAMMA gira nel ring\n\n\r");
}

void allowStopExecution(){
	usleep(1000);
}

int dir;
int prev;
int dir;
int dirOld;

void thinkDirection(){
	if      ( fastReadGpio(SENS_L) == BLACK ) dir = SENS_L;
	else if ( fastReadGpio(SENS_R) == BLACK ) dir = SENS_R;
	else if ( fastReadGpio(SENS_C) == BLACK ){
		if(prev==0) 	dir = SENS_L;
		else 		dir = SENS_R;
	}
	else 			dir = FORWARD;
}

void doSensL(){
	indietro();
	usleep(300000);
	ruotaDX();
	prev=0;
}

void doSensR(){
	indietro();
	usleep(300000);
	ruotaSX();
	prev=0;
}

void run(){

	for(;;){
		allowStopExecution();
		thinkDirection();

		if (dirOld != dir){
			dirOld=dir;

			switch(dir){
				case SENS_L:
					doSensL();
					break;
				case SENS_R:
					doSensR();
					break;
				case FORWARD:
					forward();
					break;
				default:
					printf("error\n\r");
					break;
			}

		}
	}
	closePwm();
}

int main(){

	printBanner();
	init();
	run();
	return 0;
}
