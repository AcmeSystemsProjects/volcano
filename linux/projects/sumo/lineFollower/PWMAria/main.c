#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DELAY 10000 // delay

void printBanner(){
	printf("\n\n\rPROGRAMMA gira nel ring\n\n\r");
}


void pwmOff(){
	system("echo 0 > /sys/class/pwm/pwmchip0/pwm0/enable");
	system("echo 0 > /sys/class/pwm/pwmchip0/pwm1/enable");
	usleep(DELAY);
}
void pwmOn(){
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable");
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm1/enable");
	usleep(DELAY);
}


void avanti(){
	printf("AVANTI\n\r");
	pwmOff();
	pwmOn();
	system("echo  1100000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1900000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
}

void indietro(){
	printf("INDIETRO\n\r");
	pwmOff();
	pwmOn();
	system("echo  15500000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1450000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
}

void ruotaSX(){
	printf("SX\n\r");
	system("echo  1600000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1600000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
}

void ruotaDX(){
	printf("DX\n\r");
	system("echo  1400000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1400000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
}

void initPwm(){
	printf("InitPwm\n\r");
	system("echo 0 > /sys/class/pwm/pwmchip0/export");
	system("echo 1 > /sys/class/pwm/pwmchip0/export");
	system("echo 20000000 > /sys/class/pwm/pwmchip0/pwm0/period");
	system("echo 20000000 > /sys/class/pwm/pwmchip0/pwm1/period");
	pwmOff();
}

void closePwm(){
	printf("Chiudo pwm\n\r");
	pwmOff();
	printf("unexport\n\r");
	system("echo 0 > /sys/class/pwm/pwmchip0/unexport");
	system("echo 1 > /sys/class/pwm/pwmchip0/unexport");
	sleep(1);
}


#define SCAN 0
#define CHECK 1
#define STOP 2

#define NEW 1
#define CURRENT 0

#define S_A 3
#define S_C 1 // 37, PC1
#define S_R 2 // 35, PC2
#define S_L 0 // 39, PC0
#define BLACK 1


#define SENS_DX 1
#define SENS_CX 10
#define SENS_SX 100
#define SENS_CLEAN 0
#define SENS_HOME 111


#define POS_START    0
#define POS_HOME     1
#define POS_STRISCIA 2
#define POS_BAD      3
#define POS_IMP      4

int dir;
void run(){
	int usNow,i,mm;
	long usTmp;
	int vel = SCAN;
	int line=NEW;
	closePwm();
	sleep(1);
	initPwm();

	if (init_memoryToIO()) {
                printf ("Error in init_memoryToIO() \n");
                exit(1);
        }

	setGpioinInput(S_R);
	setGpioinInput(S_C);
	setGpioinInput(S_L);

	sleep(2);
	pwmOff();

	int striscia;
	int strisciaOld=SENS_CX;
	int position=POS_START;
	int lastDir=SENS_CLEAN;
	mm=50;

        for(;;){
		usleep(1000);
		striscia = SENS_CLEAN;
		if ( fastReadGpio(S_L) == BLACK ) striscia += SENS_DX;
		if ( fastReadGpio(S_R) == BLACK ) striscia += SENS_SX;
		if ( fastReadGpio(S_C) == BLACK ) striscia += SENS_CX;
			if (strisciaOld != striscia){
			strisciaOld=striscia;
				switch (striscia){
				case SENS_CX:
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
					break;
				case SENS_DX:
					switch(position){
						case POS_HOME:
							printf("ero sulla home, continuo a girare x cercare la striscia\n\r");
							break; // continua a cercare la striscia
						case POS_STRISCIA:
							printf("ero uscito di strada, striscia ritrovata, correggo la rotta\n\r");
							ruotaSX();
							lastDir=SENS_DX;
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
					break;
				case SENS_SX:
					switch(position){
						case POS_HOME:
							printf("ero sulla home, contirnuo a girare x cercare la striscia\n\r");
							break;// continua a cercare la striscia
						case POS_STRISCIA:
							printf("ero uscito di strada, striscia ritrovata, correggo la rotta\n\r");
							ruotaDX();
							lastDir=SENS_SX;
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
					break;
				case SENS_CLEAN:
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
							// PREEMPTIVE
							//if (lastDir==SENS_DX) ruotaSX();
							//else if (lastDir==SENS_SX) ruotaDX();
							break;
						default:
							printf("imprevisto\n\r");
							position=POS_BAD;
							break; // imprevisto
					}
					break;
				case SENS_HOME:
					switch(position){
						case POS_START:
							printf("sono sulla home per la prima volta, si parte !!\n\r");
							position = POS_HOME;
							sleep(2);
							pwmOn();
							ruotaSX();
							lastDir=SENS_DX;
							printf("cerco la striscia sul sensore centrale\n\r");
							break;
						case POS_STRISCIA:
							printf("sono arrivato sulla home, mi fermo\n\r");
							position = POS_HOME;
							pwmOff();
							sleep(2);
							pwmOn();
							ruotaSX();
							lastDir=SENS_DX;
							break;
						case POS_HOME:
							break; // continua a girare x cercare la striscia
						default:
							printf("imprevisto\n\r");
							position=POS_BAD;
							break; // imprevisto
					}
					break;
				default:
					// solo due sensori
					if (position == POS_HOME) {
						break; // continua a girare
					}
					else break;
				}
			}
       		}
	closePwm();
}

int main(){

	printBanner();
	run();
	return 0;
}
