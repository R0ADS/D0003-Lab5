#ifndef THREADSTRUCT_H_
#define THREADSTRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include "Gui.h"

typedef struct{
    int serial_port;
    void *Sim;
} Thread;

#define initThreadArgs(serialPort, Sims) ((Thread){serialPort, Sims});

#endif /* THREADSTRUCT_H_ */