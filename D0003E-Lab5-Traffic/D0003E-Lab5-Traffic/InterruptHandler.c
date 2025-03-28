/*
 * InterruptHandler.c
 *
 * Created: 2025-03-25 22:21:11
 *  Author: albin
 */ 

#include <avr/io.h>
#include "InterruptHandler.h"

// Simply calls readvalue. (We want this seperate object since multiple interrupts can happen in short proximity or simultaneously)
int interrupter(InterruptHandler *self) {
	
	ASYNC(self->handler, readValue, UDR0);
	return 0;
}