// Author: HardElettroSoft
// This program test: thread, input event, trigger, hardware of Volcano board registered in Linux on /sys/class/
// compile with gcc pulse_buzzer.c -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <linux/input.h>
 
#define LED_BRIGHTNESS    "/sys/class/leds/motherboard_buzzer/brightness"
#define    LED_TRIGGER    "/sys/class/leds/motherboard_buzzer/trigger"
#define    INPUT_EVENT    "/dev/input/event0"
#define    LED_MAX_SPEED    10
#define    PERIOD_COEFF    16000
 
unsigned int led_speed;
pthread_mutex_t lock;
 
/* Blink LED */
static void *LEDMod(void *dummy)
{
    unsigned int led_period;
    int tmp;
    tmp = open(LED_BRIGHTNESS, O_WRONLY);
    if (tmp < 0)
        exit(1);
    while (1) {
        pthread_mutex_lock(&lock);
        led_period = (LED_MAX_SPEED - led_speed) * PERIOD_COEFF;
        pthread_mutex_unlock(&lock);
 
        write(tmp, "1", 2);
        usleep(led_period);
        write(tmp, "0", 2);
        usleep(led_period);
    }
}
 
int main()
{
    pthread_t pth;
    struct input_event ev;
    int tmp;
    int key_code;
    int size = sizeof(ev);
 
    /* Configure LED */
    led_speed = 5;
    tmp = open(LED_TRIGGER, O_WRONLY);
    if (tmp < 0)
        return 1;
/*    if (write(tmp, "default-on", 10) != 10) {
        printf("Error writing trigger");
        return 1;
    }*/
    close(tmp);
    printf("Configured LED for use\n");
 
    /* Create thread */
    pthread_mutex_init(&lock, NULL);
    pthread_create(&pth, NULL, LEDMod, "Blinking LED...");
 
    /* Read event0 */
    tmp = open(INPUT_EVENT, O_RDONLY);
    if (tmp < 0) {
        printf("\nOpen " INPUT_EVENT " failed!\n");
        return 1;
    }
    /* Read and parse event, update global variable */
    while (1) {

	// wait for event, stand-by
        if (read(tmp, &ev, size) < size) { 
            printf("\nReading from " INPUT_EVENT " failed!\n");
            return 1;
        }

	// ok now we have an event.

        if (ev.value == 0 && ev.type == 1) {    /* ev.value == 0 ->  Down press only; ev.value == 1 -> al rilascio */
            key_code = ev.code;
  	    printf("\n\rKey_code = %d", key_code);

            if (key_code == KEY_1) {    /* lower speed */
		printf(", P1\n\r");
                /* Protect from concurrent read/write */
                pthread_mutex_lock(&lock);
                if (led_speed > 0)
                    led_speed -= 1;
                pthread_mutex_unlock(&lock);
            } else if (key_code == KEY_2) {    /* raise speed */
		printf(", P2\n\r");
                pthread_mutex_lock(&lock);
                if (led_speed < 9)
                    led_speed += 1;
                pthread_mutex_unlock(&lock);
            }
            printf("Speed: %i\n\r", led_speed);
            usleep(1000);
        }
    }
}
