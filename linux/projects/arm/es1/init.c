
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>


void pwmDelayTimeInit(){
    int i;
    for(i=0;i<15;i++){ // pwm ch of PCA9685
        pwmTonStartTime(i);
    }
    printf("--------------------\n\r");
}

void pwmInitialPosition(){
    pinzaClose();
    braccioChiuso(20);
    baseRuota(1,90);       sleep(1);
}

void pwmInit(){
    pwmSleep();
    pwm20ms();
    pwmOn();
}

extern float mBaseAtt;
extern float m1Att;
extern float m2Att;
extern float m3Att;
extern float mRotPinzaAtt;


void initVariable(){
    // braccio chiuso
    mBaseAtt=90;
    m1Att=98;
    m2Att=97;
    m3Att=22;
    mRotPinzaAtt=93;

}

void init() {
    initI2C();
    pwmInit();
    pwmDelayTimeInit();
    pwmInitialPosition();
}

void endSettings(){
    pwmFinalPosition();
    pwmSleep();
    closeI2C();
}