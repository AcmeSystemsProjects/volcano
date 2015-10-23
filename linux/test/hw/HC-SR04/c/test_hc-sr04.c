// Author: HardElettroSoft
// You need ncurses installed
// compile with gcc test_hc-sr04.c -lncurses

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>


int readHcsr04(){
	FILE *o;
	int x;
	int us;

	if((o=fopen("/sys/class/hcsr04/value","r"))!=NULL){
		fscanf(o,"%d",&x);
		us=x;
		fclose(o);
	}
	else {
		printf("\n\rError read hcsr04\n\r");
		exit(-1);
	}
	return us;
}



int main(){
	int usSetpoint, usRange;

 	initscr(); // ncurses

	printBanner();
	sleep(1);

	// ask for setpoint
	usSetpoint=getUsSetpoint();
	//ask for range
	usRange=getRange();

	if (usRange < usSetpoint ){
		printw("Ok.\n\n\r");
		run(usSetpoint,usRange);
	}
	else{
		printw("You can not setup this value\n\r");
	}

	waitExit();
 	endwin();
	return 0;
}
