
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

int getUsSetpoint(){

    float cm;
    char key;
    int us;

    timeout(100);// 0.5sec.

    printw("Press Enter for setup setpoint\n\r");
    do{
        us=readHcsr04();
        cm=(float)us/58;
        printw("%0.1f cm       \r",cm); //do not insert  \n, it give problems. Why?
        refresh();
        key=getch(); //getch from ncurses; getchar is blocking !!, no pressed return 255.

    }while(key!=10);

    printw("Selected setpoint is %0.1f cm, %d us\n\n\r",(float)us/58,us);


    return us;
}

void printBanner(){
    printw("\n\n\rTest programs for HC-SR04\n\n\r");
}

int getRange(){
    int cmRange;
    char str[80];

    timeout(-1);
    printw("Insert tolerance in cm without decimal part; es.: 10 for 10,0 cm\n\r");
    getstr(str);
    cmRange=atoi(str);
    printw("You have set : %d\n\r",cmRange);
    return (cmRange*58);
}


void run(int usSet, int usDelta){
    int key;
    int usNow,i;
    long usTmp;
    printw("Ceck distance from setpoint\n\r");

    timeout(100);
    printw("\n\rPress Enter for exit\n\r");
    do{
        usTmp=0;
        for(i=0;i<10;i++ ){ usTmp+=readHcsr04(); usleep(1000);}
        usNow=usTmp/10;

        if (usNow>(usSet+usDelta)) printw(" You are too far                \r");
        else if (usNow<(usSet-usDelta)) printw("You are too close         \r");
        else printw("You are in range                \r");
        refresh();

        key=getch();
    }while(key!=10);
}

void waitExit(){
    int key;

    timeout(200);
    printw("\n\rPress Enter for exit\n\r");
    do{
        key=getch();
    }while(key!=10);
}