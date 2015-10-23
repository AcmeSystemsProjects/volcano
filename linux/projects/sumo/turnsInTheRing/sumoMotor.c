#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define D_MARCIA 10000 // delay prima di cambiare direzione


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


void forward(){
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
