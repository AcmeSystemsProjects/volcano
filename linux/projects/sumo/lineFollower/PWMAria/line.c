#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gpio.h"
#include "line.h"



extern int position;
extern int striscia;


void doSensCx(){
	switch(position){
		case POS_HOME:
			printf("ero sulla home, comincio\n\r");
			position=POS_STRISCIA;
			avanti();
			break;
		case POS_STRISCIA:
			printf("ero sulla striscia, continuo\n\r");
			position=POS_STRISCIA;
			avanti();
			break;
		case POS_START:
			printf("mal posizionato, dovevo essere sulla home\n\r");
			position=POS_BAD;
			break; // mal posizionato
		default:
			printf("imprevisto\n\r");
			position=POS_BAD;
			break; // imprevisto
	}
}

void doSensDx(){
	switch(position){
		case POS_HOME:
			printf("ero sulla home, continuo a girare x cercare la striscia\n\r");
			break; // continua a cercare la striscia
		case POS_STRISCIA:
			printf("ero uscito di strada, striscia ritrovata, correggo la rotta\n\r");
			ruotaSX();
			break;
		case POS_START:
			printf("mal posizionato, devo essere sulla home\n\r");
			position=POS_BAD;
			break; // mal posizionato
		default:
			printf("imprevisto\n\r");
			position=POS_BAD;
			break; // imprevisto
	}
}

void doSensSx(){
	switch(position){
		case POS_HOME:
			printf("ero sulla home, contirnuo a girare x cercare la striscia\n\r");
			break;// continua a cercare la striscia
		case POS_STRISCIA:
			printf("ero uscito di strada, striscia ritrovata, correggo la rotta\n\r");
			ruotaDX();
			break;
		case POS_START:
			printf("mal posizionato, dovevo essere sulla home\n\r");
			position=POS_BAD;
			break; // mal posizionato
		default:
			printf("imprevisto\n\r");
			position=POS_BAD;
			break; // imprevisto
	}
}

void doSensClean(){
	switch(position){
		// se era sulla home
		case POS_HOME:
			printf("non leggo la striscia, ero nella home\n\r");
			break; // continua a girare
		// se parte per la prima volta
		case POS_START:
			printf("non leggo la striscia, appena acceso, mal posizionato, dovevo essere sulla home\n\r");
			break;  // imprevisto
		// se era sulla striscia
		case POS_STRISCIA:
			printf("uscito di strada, ero sulla striscia\n\r");
			break;
		default:
			printf("imprevisto\n\r");
			position=POS_BAD;
			break; // imprevisto
	}
}

void doSensHome(){
	switch(position){
		case POS_START:
			printf("sono sulla home per la prima volta, si parte !!\n\r");
			position = POS_HOME;
			sleep(2);
			pwmOn();
			ruotaSX();
			printf("cerco la striscia sul sensore centrale\n\r");
			break;
		case POS_STRISCIA:
			printf("sono arrivato sulla home, mi fermo\n\r");
			position = POS_HOME;
			pwmOff();
			sleep(2);
			pwmOn();
			ruotaSX();
			break;
		case POS_HOME:
			break; // continua a girare x cercare la striscia
		default:
			printf("imprevisto\n\r");
			position=POS_BAD;
			break; // imprevisto
	}
}

int strisciaRead(){
	int striscia = SENS_CLEAN;
	if ( fastReadGpio(S_L) == BLACK ) striscia += SENS_DX;
	if ( fastReadGpio(S_R) == BLACK ) striscia += SENS_SX;
	if ( fastReadGpio(S_C) == BLACK ) striscia += SENS_CX;
	return striscia;
}

void strisciaProgram(){
	switch (striscia){
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
			// solo due sensori
			if (position == POS_HOME) {
				break; // continua a girare
			}
			else break;
	}
}

