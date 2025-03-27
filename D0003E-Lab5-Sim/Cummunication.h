#ifndef CUMMUNICATION_H_
#define CUMMUNICATION_H_
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "threadStruct.h"

#define initCummunication() {initObject()}

void *USART_Receiver(void *t);
void USART_Transmiter(void *t, uint8_t data);

#endif /* COMMUNICATION_H_ */