

#include <math.h>

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

// M1: 5= AVANTI, 98= INDIETRO CENTRO = 53
// M2: 20= su/disteso, 97 = giu/chiuso
// M3: 100= su, 0 = giu/CHiuso
// MBASE: 0= sx, 100 = dx
// MROTPINZA: 5=orario  95=antiorario
// MPINZA: 30=chiusa  70=aperta

//PCA9685 pwm pins
#define CH_ROT_BASE 3
#define CH_ROT_PINZA 2
#define CH_PINZA 4
#define CH_M3 13
#define CH_M2 11
#define CH_M1 12

#define MS_PERIOD 20

float mBaseAtt;
float m1Att;
float m2Att;
float m3Att;
float mRotPinzaAtt;
float mPinzaAtt;




//ch  = 0..15 by PCA9685
//PERC = 0..100
void setPwmValue(int ch, float perc){

    if (perc>100) exit(1);

    //printf("% richiesta da mot%d         = %f\n\r",ch,perc);

    float tonMs = 0.48f + 0.02f * perc; // 1/100
    //printf("Ton ms              = %f\n\r",tonMs);

    float tonPerc = tonMs/MS_PERIOD*100;
    //printf("Ton %%               = %f\n\r",tonPerc);

    int tonDec = (int)round( 40.96f * tonPerc) ; // 4096/100
    //printf("Ton dec             = %d\n\r",tonDec);

    int chPin=ch;
    if (chPin>=8) chPin -= 8;
    float delayTimeMs = 2.5f * chPin;
    //printf("Delay time ms       = %f\n\r",delayTimeMs);

    int delayTimePerc = delayTimeMs/MS_PERIOD*100;
    //printf("Delay time %%        = %d\n\r",delayTimePerc);

    long delayTimeReg =(long)round(40.96f *delayTimePerc);
    if (delayTimeReg>0) delayTimeReg-=1;

    long tOffDec = tonDec + delayTimeReg;
    int offRegL = (int)(tOffDec & 0xff);
    int offRegH = (int)(tOffDec >> 8);
    //printf("PWM Off Reg         = 0x%04x\n\r",tOffDec);
    //printf("PWM Off Reg Low     = 0x%02x\n\r",offRegL);
    //printf("PWM Off Reg High    = 0x%02x\n\r",offRegH);


    PCA9685SetOffTime(ch, offRegL, offRegH );

    //printf("------------------------\n\r");

}




void braccio(int nSteps, int mBaseDest, int m1Dest, int m2Dest, int m3Dest, int mRotPinzaDest, int mPinzaDest){

    if ( (m1Att!=m1Dest) || \
		(m2Att!=m2Dest) || \
		(m3Att!=m3Dest) || \
		(mRotPinzaAtt!=mRotPinzaDest) || \
		(mPinzaAtt!=mPinzaDest) || \
		(mBaseAtt!=mBaseDest) ){

        float mBaseStep = ( (float)mBaseAtt-(float)mBaseDest) / nSteps;
        float m1Step = ( (float)m1Att-(float)m1Dest) / nSteps;
        float m2Step = ( (float)m2Att-(float)m2Dest) / nSteps;
        float m3Step = ( (float)m3Att-(float)m3Dest) / nSteps;
        float mRotPinzaStep = ( (float)mRotPinzaAtt-(float)mRotPinzaDest) / nSteps;
        float mPinzaStep = ( (float)mPinzaAtt-(float)mPinzaDest) / nSteps;

        int i;
        for(i = 0; i<nSteps; i++){
            setPwmValue(CH_PINZA, mPinzaAtt);
            setPwmValue(CH_ROT_PINZA, mRotPinzaAtt);
            setPwmValue(CH_ROT_BASE, mBaseAtt);
            setPwmValue(CH_M1, m1Att);
            setPwmValue(CH_M2, m2Att);
            setPwmValue(CH_M3, m3Att);
            mBaseAtt -= mBaseStep;
            m1Att -= m1Step;
            m2Att -= m2Step;
            m3Att -= m3Step;
            mRotPinzaAtt -= mRotPinzaStep;
            mPinzaAtt -= mPinzaStep;
            usleep(10);
        }
    }
    //fix possible error in division of float
    mBaseAtt = mBaseDest;
    m1Att = m1Dest;
    m2Att = m2Dest;
    m3Att = m3Dest;
    mRotPinzaAtt = mRotPinzaDest;
    mPinzaAtt = mPinzaDest;

    setPwmValue(CH_ROT_BASE, mBaseAtt);
    setPwmValue(CH_M1, m1Att);
    setPwmValue(CH_M2, m2Att);
    setPwmValue(CH_M3, m3Att);
    setPwmValue(CH_ROT_PINZA, mRotPinzaAtt);
    setPwmValue(CH_PINZA, mPinzaAtt);
}



#define CLOSE 32 // 30 is too much close to min value
#define POLY  40
#define APERTA  70
void baseRuota(int steps, int rot){
    braccio(steps, rot, m1Att, m2Att, m3Att, mRotPinzaAtt,mPinzaAtt);
}
void pinza(int obj){
    braccio(2, mBaseAtt, m1Att, m2Att, m3Att, mRotPinzaAtt,obj);
}
void pinzaRot(int steps, int rot){
    braccio(steps, mBaseAtt, m1Att, m2Att, m3Att, rot,mPinzaAtt);
}
void braccioChiuso(int step){	   braccio(step,mBaseAtt,     98, 97, 25,    	             93,mPinzaAtt); sleep(1); }
void braccioAlzato(){      	   braccio(100 ,mBaseAtt,     53, 20, 58,          mRotPinzaAtt,mPinzaAtt); sleep(1); }
void braccioSgancia(){      	   braccio(130 ,mBaseAtt,     30, 40, 60,                    50,mPinzaAtt); sleep(1); }


void braccioEsulta2(){      	   braccio( 10 ,mBaseAtt,     53, 20, 58,                    50,mPinzaAtt);  }
void braccioShowObj(){      	   braccio(100 ,mBaseAtt,     83, 60, 45,                    50,mPinzaAtt); sleep(1); }
void braccioEsulta1(){      	   braccio( 30 ,mBaseAtt,     83, 60,100,                    50,mPinzaAtt);  }
void braccioEsulta3(){      	   braccio(  1 ,mBaseAtt,     m1Att, m2Att, 45,              50,mPinzaAtt);  }
void braccioEsulta4(){      	   braccio(  1 ,mBaseAtt,     m1Att, m2Att, 55,              50,mPinzaAtt);  }

void braccioAvantiMax(int step){
    braccio( step  , mBaseAtt,     12, 30, 72,    50,mPinzaAtt);
    braccio( step/2, mBaseAtt,      5, 20, 63,    50,mPinzaAtt);
    sleep(1);
}
void braccioAvantiMin(int step){
    braccio( step  ,mBaseAtt,      23, 47, 70,    50,mPinzaAtt);
    braccio( step/2,mBaseAtt,      35, 88,100,    50,mPinzaAtt);
    sleep(1);
}



void pwmSleep(){
    PCA9685Sleep();
    usleep(1000); // > 500uS
}

void pwm20ms(){
    PAC9685SetPeriod();
}
void pwmOn(){
    PAC9685PwmOn();
}


extern float mPinzaAtt;
void setStartPositionPinza(){
    mPinzaAtt=CLOSE;
}

void pwmFinalPosition(){
    pinza(CLOSE);
    braccioChiuso(100);
    baseRuota(1,90);       sleep(1);
}

void pinzaClose(){
    pinza(CLOSE);
}

void pinzaOpen(){
    pinza(CLOSE);
}

void pinzaPoly(){
    pinza(POLY);
}


void pwmTonStartTime(int ch){ // 0..15

    int chPin=ch;
    if (chPin>=8) chPin -= 8;
    float delayTimeMs = 2.5f * chPin;
    printf("Delay time ms       = %f\n\r",delayTimeMs);

    int delayTimePerc = delayTimeMs/MS_PERIOD*100;
    printf("Delay time %%        = %d\n\r",delayTimePerc);

    long delayTimeReg =(long)round(40.96f *delayTimePerc);
    if (delayTimeReg>0) delayTimeReg-=1;
    int onRegL = (int)(delayTimeReg & 0xff);
    int onRegH = (int)(delayTimeReg >> 8);
    printf("Delay time Reg      = 0x%04x\n\r",delayTimeReg);
    printf("Delay time Reg Low  = 0x%02x\n\r",onRegL);
    printf("Delay time Reg High = 0x%02x\n\r",onRegH);


    PCA9685SetOnTime(ch, onRegL, onRegH );
}
