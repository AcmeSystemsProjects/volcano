#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DELAY 10000 // delay


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

void forward(){
	printf("forward\n\r");
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

void turnSX(){
	printf("SX\n\r");
	system("echo  1600000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system("echo  1600000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
}

void turnDX(){
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

void closePwmIfOpened(){
        closePwm();
}



