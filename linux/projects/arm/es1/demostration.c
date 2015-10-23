
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

void stretching(){
    int i;

    baseRuota(200,0);
    braccioAlzato();
    sleep(1);
    for(i=0;i<2;i++){
        pinzaOpen();
        usleep(700000);
        pinzaClose();
        usleep(700000);
    }

    baseRuota(20,90);
    braccioChiuso(100);
    usleep(500000);
}


void happy(){
    int i;

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
}

void takeObj(){
    braccioAvantiMin(60);
    pinzaOpen();
    sleep(5);

    // base, m1, m2, m3, mRotPinza, mPinza
    braccioAvantiMax(90);
    pinzaPoly();
    usleep(300000);

    braccioShowObj(50);
    sleep(1);
}

void releases(){
    //ruota e sgancia
    baseRuota(100,0);
    braccioSgancia();
    sleep(1);
    pinzaOpen();
    sleep(1);
}

void thanks(){
    int w,i;
    for(w=40;w<=80;w+=20){
        printf("w = %d \n\r",w);
        baseRuota(100,w);

        int j;
        for(i=0;i<3;i++){
            pinzaOpen();
            braccioEsulta1();
            usleep(100000);
            pinzaClose();
            braccioEsulta2();
            usleep(100000);

            for(j=0;j<3;j++){
                braccioEsulta3();
                pinzaClose();
                usleep(300000);

                pinzaOpen();
                braccioEsulta4();
                usleep(300000);
            }
        }
    }
    sleep(1);
}

void demostration(){

    stretching();
    takeObj();
    happy();
    releases();
    thanks();

}