#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void forward(){
    system("echo  1100000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
    system("echo  1900000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
    system("echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable");
    system("echo 1 > /sys/class/pwm/pwmchip0/pwm1/enable");
}

void turnMax(){
    system("echo  1900000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
    system("echo  1900000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
    system("echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable");
    system("echo 1 > /sys/class/pwm/pwmchip0/pwm1/enable");
}
void turnMin(){
    system("echo  1400000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
    system("echo  1400000 > /sys/class/pwm/pwmchip0/pwm1/duty_cycle");
    system("echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable");
    system("echo 1 > /sys/class/pwm/pwmchip0/pwm1/enable");
}


void stop(){
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
    stop();
    system("echo 0 > /sys/class/pwm/pwmchip0/unexport");
    system("echo 1 > /sys/class/pwm/pwmchip0/unexport");
}
