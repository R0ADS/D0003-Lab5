/*
 * Communication.h
 *
 * Created: 2025-03-25 17:21:46
 *  Author: albin
 */ 

#include <stdint.h>

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "TinyTimber.h"

typedef struct {
	Object super;
} Communication;

#define initCommunication() {initObject()}

int transmit(Communication *self, int data);

#endif /* COMMUNICATION_H_ */