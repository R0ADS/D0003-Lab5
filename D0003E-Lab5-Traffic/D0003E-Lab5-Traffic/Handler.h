/*
 * Handler.h
 *
 * Created: 2025-03-25 17:50:05
 *  Author: albin
 */ 


#ifndef HANDLER_H_
#define HANDLER_H_

#include "TinyTimber.h"

typedef struct {
	Object super;
	Communication *com;
	Lcd *lcd;
	int isNorth;
	int southQueue;
	int northQueue;
	int onBridge;
} Handler;

#define initHandler(com, lcd) {initObject(), com, lcd, 1, 0, 0, 0}

#endif /* HANDLER_H_ */