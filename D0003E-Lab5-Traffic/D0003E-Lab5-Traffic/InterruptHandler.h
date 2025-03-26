/*
 * InterruptHandler.h
 *
 * Created: 2025-03-25 22:21:27
 *  Author: albin
 */ 


#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_

#include "TinyTimber.h"
#include "Handler.h"

typedef struct {
	Object super;
	Handler *handler;
} InterruptHandler;

#define initInterruptHandler(handler) {initObject(), handler}

int interrupter(InterruptHandler *self);


#endif /* INTERRUPTHANDLER_H_ */