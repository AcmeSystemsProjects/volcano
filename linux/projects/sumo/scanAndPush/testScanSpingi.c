#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


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


void printBanner(){
	printf("\n\n\rPROGRAMMA rileva robot entro 70cm\n\n\r");
}


void avanti(){
	system("echo  1100000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1900000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable");
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm1/enable");
}

void ruotaMax(){
	system("echo  1900000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1900000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable");
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm1/enable");
}
void ruotaMin(){
	system("echo  1400000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1400000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable");
	system("echo 1 > /sys/class/pwm/pwmchip0/pwm1/enable");
}


void fermo(){
	system("echo 0 > /sys/class/pwm/pwmchip0/pwm0/enable");
	system("echo 0 > /sys/class/pwm/pwmchip0/pwm1/enable");
}

void initPwm(){
	printf("InitPwm\n\r");
	system("echo 0 > /sys/class/pwm/pwmchip0/export");
	system("echo 1 > /sys/class/pwm/pwmchip0/export");
	system("echo 20000000 > /sys/class/pwm/pwmchip0/pwm0/period");
	system("echo 20000000 > /sys/class/pwm/pwmchip0/pwm1/period");
}

void closePwm(){
	printf("Chiudo pwm\n\r");
	fermo();
	system("echo 0 > /sys/class/pwm/pwmchip0/unexport");
	system("echo 1 > /sys/class/pwm/pwmchip0/unexport");
}


#define SCAN 0
#define CHECK 1
#define STOP 2

#define NEW 1
#define CURRENT 0

int dir;
void run(){
	int usNow,i,mm;
	long usTmp;
	int vel = SCAN;
	int line=NEW;
	initPwm();
	fermo();
	sleep(2);

        for(;;){
		usNow=readHcsr04();
		usleep(10000);
		mm=(int)(usNow/58);
		if ( (mm > 55) || ( mm < 1 ) ){
			if (line == CURRENT ) {
				printf("\n\r");
				line = NEW;
			}

			if ( vel == STOP ) vel = SCAN;
			if ( vel == SCAN) ruotaMax();
			else ruotaMin(); // CHECK

			if (mm > 55 )	printf("mm: %d                 \n\r", mm);
			else 		printf("------                 \n\r", mm);


		}
	// da qui in poi: mm < 55, robot beccato, ma potrebbe non essersi fermato di fronte,
	//  allora inverte rotta
		else {
			if (line == NEW ) {
				printf("\n\r");
				line = CURRENT;
			}

			if (vel == SCAN)  {
				printf("Cerco meglio il robot       \r");
				ruotaMin();
				vel= CHECK;
			}
			else if( vel==CHECK ){ // robot ritrovato
				avanti();
				vel= STOP; 	// non appena si muove, deve riprendere a
						//   ruotare velocemtne
				printf("\n\rRobot trovato        \n\r");
 			}
			// STOP -> NULLA
        	}
	}
	closePwm();
}

int main(){

	printBanner();
	run();
	return 0;
}
