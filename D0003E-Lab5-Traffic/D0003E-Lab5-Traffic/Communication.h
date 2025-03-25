/*
 * Communication.h
 *
 * Created: 2025-03-25 17:21:46
 *  Author: albin
 */ 

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "TinyTimber.h"

typedef struct {
	Object super;
} Communication;

#define initCommunication() {initObject()}

uint8_t receive(void);

uint8_t transmit(uint8_t data);

#endif /* COMMUNICATION_H_ */