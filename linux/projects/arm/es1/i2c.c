#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
//#include <getopt.h>
//#include <time.h>


#define I2C_ADDR 0x5F

#define REG_MODE1 0x00
#define REG_MODE2 0x01
#define REG_PRE_SCALE 0xFE
#define REG_LED0_ON_L 0x06
#define REG_LED0_OFF_L 0x08

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

void PCA9685Sleep(){
    i2cWrite(REG_MODE1,0x11);
}

void PAC9685SetPeriod(){
    i2cWrite(REG_PRE_SCALE,0x79);
}

void PAC9685PwmOn(){
    i2cWrite(REG_MODE1,0x00);
}

void PCA9685SetOffTime(int ch, int offRegL, int offRegH ){
    int reg = REG_LED0_OFF_L + 4 * ch;
    i2cWrite(reg, offRegL);
    i2cWrite(reg+1, offRegH);
}

void PCA9685SetOnTime(ch, onRegL, onRegH ){
    int reg = REG_LED0_ON_L + 4 * ch;
    i2cWrite(reg  , onRegL);
    i2cWrite(reg+1, onRegH);
}

void closeI2C(){
    close(fd);
}