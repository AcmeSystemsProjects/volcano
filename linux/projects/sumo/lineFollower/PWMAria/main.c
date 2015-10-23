/***********************************************************
 *
 * compile with doing cmake
 *
 * the program is for black line on white table/floor
 *
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"
#include "line.h"
#include "version.h"


void printBanner(){
	printf("\n\n\rPROGRAMMA lineFollower v%d.%d gira nel ring\n\n\r",lineFollower_VERSION_MAJOR,lineFollower_VERSION_MINOR);
}

void allowStopExecution(){
	usleep(1000);
}

int position;
int line;
int lineOld;
int dir;
int lastDir;


void run(){
	lineOld=lineRead();
        for(;;){
		allowStopExecution();
		line=lineRead();
		if (lineOld != line){
			lineOld=line;
			lineProgram();
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
