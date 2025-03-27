#include "Init.h"
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

int init(){
    int serial_port = open("/dev/ttyS3", O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (serial_port < 0) {      // error for serial port not defined
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return 1;
    }
    // Created struct for termios called tty.
    struct termios tty;

    if(tcgetattr(serial_port, &tty) != 0) {     // checking if errors with flaggs
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        close(serial_port);
        return 1;
    }
    
    cfsetospeed(&tty, B9600);   // baudspeed setting
    cfsetispeed(&tty, B9600);   

    tty.c_cflag |= CS8;         // 8 Bits
    tty.c_cflag &= ~CSTOPB;     // 1 Stop bit
    tty.c_cflag &= ~PARENB;     // no parity bit

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {      // saves tty settings and checking for errors
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        close(serial_port);
        return 1;
    }
    return serial_port;
}

