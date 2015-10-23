
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

int getmm(){
    int ret;
    int usNow;

    usNow=readHcsr04();
    ret=(int)(usNow/58);
    printf("mm: %d                 \n\r", ret);
    usleep(10000);
    return ;
}