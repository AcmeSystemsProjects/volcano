#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SCAN 0
#define CHECK 1
#define STOP 2

#define NEW 1
#define CURRENT 0

int dir;
int line;
int speed;
int mm;


void printBanner(){
	printf("\n\n\rPROGRAM for detect robot within 55cm\n\n\r");
}

void init(){
	line=NEW;
	speed = SCAN;

	initPwm();
	stop();
	sleep(2);
}

void scanFast(){
	if (line == CURRENT ) {
		printf("\n\r");
		line = NEW;
	}

	if ( speed == STOP ) speed = SCAN;
	if ( speed == SCAN) turnMax();
	else turnMin(); // CHECK
}

void scanSlow(){
	printf("Seeking better the robot         \r");
	turnMin();
	speed= CHECK;
}

void push(){
	// robot found
	forward();
	speed= STOP; 	// as soon as the object moves, the robot must rotate faster
	printf("\n\rRobot found        \n\r");
}

int objInRange(){
	if ( (mm > 55) || ( mm < 1 ) ) return 0;
	else return 1;
}

void run(){

	init();

	for(;;){
		mm=getmm();
		if ( objInRange() )	scanFast();

		// the robot could not stand still in front, so reverse the direction.
		else {
			if (line == NEW ) {
				printf("\n\r");
				line = CURRENT;
			}
			if (speed == SCAN)  scanSlow();
			else if( speed==CHECK ) push();
			// STOP -> nothing
		}
	}
	closePwm();
}

int main(){

	printBanner();
	run();
	return 0;
}
