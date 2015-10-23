#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"
#include "line.h"



extern int position;
extern int line;


void doSensCx(){
	switch(position){
		case POS_HOME:
			printf("I was on the starting point, start now\n\r");
			position=POS_LINE;
			forward();
			break;
		case POS_LINE:
			printf("I was on the line, continue\n\r");
			position=POS_LINE;
			forward();
			break;
		case POS_START:
			printf("I was not well located, I had to be on the starting point\n\r");
			position=POS_BAD;
			break; // mal posizionato
		default:
			printf("unforeseen\n\r");
			position=POS_BAD;
			break; // unforeseen
	}
}

void doSensDx(){
	switch(position){
		case POS_HOME:
			printf("ero sulla home, continuo a girare x cercare la line\n\r");
			break; // continua a cercare la line
		case POS_LINE:
			printf("ero uscito di strada, line ritrovata, correggo la rotta\n\r");
			turnSX();
			break;
		case POS_START:
			printf("I was not well located, I had to be on the starting point\n\r");
			position=POS_BAD;
			break; // mal posizionato
		default:
			printf("unforeseen\n\r");
			position=POS_BAD;
			break; // unforeseen
	}
}

void doSensSx(){
	switch(position){
		case POS_HOME:
			printf("ero sulla home, contirnuo a girare x cercare la line\n\r");
			break;// continua a cercare la line
		case POS_LINE:
			printf("ero uscito di strada, line ritrovata, correggo la rotta\n\r");
			turnDX();
			break;
		case POS_START:
			printf("I was not well located, I had to be on the starting point\n\r");
			position=POS_BAD;
			break; // mal posizionato
		default:
			printf("unforeseen\n\r");
			position=POS_BAD;
			break; // unforeseen
	}
}

void doSensClean(){
	switch(position){
		// If it was on the starting point
		case POS_HOME:
			printf("non leggo la line, ero nella home\n\r");
			break; // continue to run
		// If it start for the first time
		case POS_START:
			printf("I am not able to read the line, just start up, I am not well located, I had to be on the starting point\n\r");
			break;  // unforeseen
		// If It was on the line
		case POS_LINE:
			printf("I went out the line, I was on the line\n\r");
			break;
		default:
			printf("unforeseen\n\r");
			position=POS_BAD;
			break; // unforeseen
	}
}

void doSensHome(){
	switch(position){
		case POS_START:
			printf("I'm about to start for the first time, I can start !!\n\r");
			position = POS_HOME;
			sleep(2);
			pwmOn();
			turnSX();
			printf("I look for the line on the central line sensor\n\r");
			break;
		case POS_LINE:
			printf("I arrive on the starting point\n\r");
			position = POS_HOME;
			pwmOff();
			sleep(2);
			pwmOn();
			turnSX();
			break;
		case POS_HOME:
			break; // I still turn to look for the line
		default:
			printf("unforeseen\n\r");
			position=POS_BAD;
			break; // unforeseen
	}
}

int lineRead(){
	int line = SENS_CLEAN;
	if ( fastReadGpio(LINE_SENSOR_L) == BLACK ) line += SENS_DX;
	if ( fastReadGpio(LINE_SENSOR_R) == BLACK ) line += SENS_SX;
	if ( fastReadGpio(LINE_SENSOR_C) == BLACK ) line += SENS_CX;
	return line;
}

void lineProgram(){
	switch (line){
		case SENS_CX:
			doSensCx();
			break;
		case SENS_DX:
			doSensDx();
			break;
		case SENS_SX:
			doSensSx();
			break;
		case SENS_CLEAN:
			doSensClean();
			break;
		case SENS_HOME:
			doSensHome();
			break;
		default:
			break; // ok
	}
}

