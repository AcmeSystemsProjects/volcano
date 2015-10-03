// http://stackoverflow.com/questions/505023/reading-writing-from-using-i2c-on-linux
// https://community.freescale.com/thread/313662
//http://www.atmel.com/Images/doc32083.pdf

//480uS 2.4% -> 2480uS 12.4%

#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <math.h>
//#include <tgmath.h>
#include <time.h>

#define I2C_ADDR 0x5F

#define REG_MODE1 0x00
#define REG_MODE2 0x01
#define REG_PRE_SCALE 0xFE
#define REG_LED0_ON_L 0x06
#define REG_LED0_OFF_L 0x08
#define MS_PERIOD 20


#define CH_ROT_BASE 3
#define CH_ROT_PINZA 2
#define CH_PINZA 4
#define CH_M3 13
#define CH_M2 11
#define CH_M1 12

//I2C
int fd;
struct i2c_rdwr_ioctl_data work_queue;
struct i2c_msg msg[2];
uint8_t ret;
static const char *device = "/dev/i2c-0";	// I2C bus


static void exit_on_error (const char *s){
 	perror(s);
  	abort();
}

void initI2C(){
	// Open I2C device
       	if ((fd = open(device, O_RDWR)) < 0) exit_on_error ("Can't open I2C device");

	// Set I2C slave address
	if (ioctl(fd, I2C_SLAVE,I2C_ADDR) < 0) exit_on_error ("Can't talk to slave");
}


void i2cWrite(int reg, int value){
	uint8_t msg_data[2] = {0,0};

	work_queue.nmsgs = 1;
	work_queue.msgs = (struct i2c_msg*) malloc(work_queue.nmsgs * sizeof(struct i2c_msg));
	if (!work_queue.msgs) {
		printf("Memory alloc error\n");
		close(fd);
	}
	ioctl(fd, I2C_TIMEOUT, 2);
	ioctl(fd, I2C_RETRIES, 1);

	work_queue.msgs[0].len   = 2;
	work_queue.msgs[0].flags = 0; // 0= WRITE
	work_queue.msgs[0].addr = I2C_ADDR; /* slave address */
	msg_data[0] =reg; /* register address*/
	msg_data[1] =value; /* data */
	work_queue.msgs[0].buf = msg_data;
	if( ioctl(fd, I2C_RDWR, (unsigned long) &work_queue) < 0)  {
		printf("I2C Write error %d in register %d\n\r", value, reg);
	}

}

void pwmSleep(){
	i2cWrite(REG_MODE1,0x11);
	usleep(1000); // > 500uS
}

void pwm20ms(){
	i2cWrite(REG_PRE_SCALE,0x79);
}
void pwmOn(){
	i2cWrite(REG_MODE1,0x00);
}

void pwmInit(){
	pwmSleep();
	pwm20ms();
	pwmOn();
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

	int reg = REG_LED0_ON_L + 4 * ch; // 6+0 6+4 6+8 6+12 

	i2cWrite(reg  , onRegL);
	i2cWrite(reg+1, onRegH);
}

void pwmDelayTimeInit(){
	int i;
	for(i=0;i<15;i++){ // pwm ch of PCA9685
		 pwmTonStartTime(i);
	}
	printf("--------------------\n\r");
}


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

	int reg = REG_LED0_OFF_L + 4 * ch; // 6+0 6+4 6+8 6+12 
	i2cWrite(reg, offRegL);
	i2cWrite(reg+1, offRegH);

	//printf("------------------------\n\r");

}

float mBaseAtt;
float m1Att;
float m2Att;
float m3Att;
float mRotPinzaAtt;
float mPinzaAtt;

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

// M1: 5= AVANTI, 98= INDIETRO CENTRO = 53
// M2: 20= su/disteso, 97 = giu/chiuso
// M3: 100= su, 0 = giu/CHiuso
// MBASE: 0= sx, 100 = dx
// MROTPINZA: 5=orario  95=antiorario
// MPINZA: 30=chiusa  70=aperta


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
void pwmInitialPosition(){
	pinza(CLOSE);
	braccioChiuso(20);
	baseRuota(1,90);       sleep(1);
}
void pwmFinalPosition(){
	pinza(CLOSE);
	braccioChiuso(100);
	baseRuota(1,90);       sleep(1);
}

// srand((unsigned int)time(NULL)); // http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c
int main(){
	int i;

	// braccio chiuso
	mBaseAtt=90;
	m1Att=98;
	m2Att=97;
	m3Att=22;
	mRotPinzaAtt=93;
	mPinzaAtt=CLOSE;

	initI2C();
	pwmInit();
	pwmDelayTimeInit();
	pwmInitialPosition();
//for(;;){

	baseRuota(200,0);
	braccioAlzato();       sleep(1);
	for(i=0;i<2;i++){
		pinza(APERTA);
		usleep(700000);
		pinza(CLOSE);
		usleep(700000);
	}

	baseRuota(20,90);
	braccioChiuso(100);    usleep(500000);

	braccioAvantiMin(60);
	pinza(APERTA);
	sleep(5);

	// base, m1, m2, m3, mRotPinza, mPinza
	braccioAvantiMax(90);
	pinza(POLY);
	usleep(300000);

	braccioShowObj(50);
	sleep(1);


	//sborone
	baseRuota(30,45);
	pinzaRot(100,5);
	sleep(1);
	pinzaRot(100,95);
	sleep(1);
	for(i=0;i<4;i++){
		pinzaRot(2,5);
		usleep(500000);
		pinzaRot(2,95);
		usleep(500000);
	}
	sleep(1);

	//ruota e sgancia
	baseRuota(100,0);
	braccioSgancia();
	sleep(1);
	pinza(APERTA);
	sleep(1);


	//ringraziamenti

	int w;
	for(w=40;w<=80;w+=20){
		printf("w = %d \n\r",w);
		baseRuota(100,w);

		int j;
		for(i=0;i<3;i++){
			pinza(APERTA);
			braccioEsulta1();
			usleep(100000);
			pinza(CLOSE);
			braccioEsulta2();
			usleep(100000);

			for(j=0;j<3;j++){
				braccioEsulta3();
				pinza(CLOSE);
				usleep(300000);

				pinza(APERTA);
				braccioEsulta4();
				usleep(300000);
			}
		}
	}
	sleep(1);

	pwmFinalPosition();
	pwmSleep();
	close(fd);
	return 0;
}
