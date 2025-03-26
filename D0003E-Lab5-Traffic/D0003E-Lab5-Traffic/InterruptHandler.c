/*
 * InterruptHandler.c
 *
 * Created: 2025-03-25 22:21:11
 *  Author: albin
 */ 

#include<avr/io.h>
#include "InterruptHandler.h"

int interrupter(InterruptHandler *self) {
	/*while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = 0b1001;*/
	ASYNC(self->handler, readValue, UDR0);
	return 0;
}