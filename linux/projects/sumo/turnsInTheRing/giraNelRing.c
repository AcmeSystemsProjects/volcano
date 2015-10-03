#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gpioFastLib.c"


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

#define D_MARCIA 10000 // delay prima di cambiare direzione

void printBanner(){
	printf("\n\n\rPROGRAMMA gira nel ring\n\n\r");
}


void pwmOff(){
	system("echo 0 > /sys/class/pwm/pwmchip0/pwm0/enable");
	system("echo 0 > /sys/class/pwm/pwmchip0/pwm1/enable");
	usleep(D_MARCIA);
}
void pwmOn(){
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable");
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm1/enable");
	usleep(D_MARCIA); // x evitare che cambi troppo bruscamente
}


void avanti(){
	printf("AVANTI\n\r");
	pwmOff();
	//rampa
	pwmOn();
	system("echo  1450000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1550000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
	usleep(D_MARCIA);
	system("echo  1300000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1700000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
	usleep(D_MARCIA);
	system("echo  1100000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1900000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
	usleep(D_MARCIA);

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
	pwmOff();
	pwmOn();
	system("echo  1550000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1550000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
}

void ruotaDX(){
	printf("DX\n\r");
	pwmOff();
	pwmOn();
	system("echo  1450000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1450000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
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



	indietro();
	sleep(2);
	ruotaDX();
	sleep(2);
	ruotaSX();
	sleep(2);
	pwmOff();


	sleep(1);

	int prev=0;
	int dir=S_A;
	int dirOld=S_C;
	mm=50;


        for(;;){
		usleep(1000);
	//	usNow=readHcsr04();
	//	usleep(20000);//10000
	//	mm=(int)(usNow/58);
		if ( (mm < 10) && ( mm > 1 ) ){
			printf("Trovato ostacolo\n\r", mm);
			pwmOff();
			sleep(1);
		}
		else if (mm > 10){
			if      ( fastReadGpio(S_L) == BLACK ) dir = S_L;
			else if ( fastReadGpio(S_R) == BLACK ) dir = S_R;
			else if ( fastReadGpio(S_C) == BLACK ){
				if(prev==0) 	dir = S_L;
				else 		dir = S_R;
			}
			else 			dir = S_A;

			if (dirOld != dir){
				dirOld=dir;

				if (dir==S_L){
					indietro();
					usleep(300000);
					ruotaDX();
					prev=0;
				}
				else if (dir==S_R){
					indietro();
					usleep(300000);
					ruotaSX();
					prev=0;
				}
				else if (dir == S_A){
					avanti();
				}
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
