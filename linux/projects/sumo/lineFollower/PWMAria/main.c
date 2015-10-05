#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"
#include "line.h"

void printBanner(){
	printf("\n\n\rPROGRAMMA gira nel ring\n\n\r");
}



void allowStopExecution(){
	usleep(1000);
}

int position;
int striscia;
int strisciaOld;
int dir;
int lastDir;


void run(){
	strisciaOld=strisciaRead();
        for(;;){
		allowStopExecution();
		striscia=strisciaRead();
		if (strisciaOld != striscia){
			strisciaOld=striscia;
			strisciaProgram();
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
