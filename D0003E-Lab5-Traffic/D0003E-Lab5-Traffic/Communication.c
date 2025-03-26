/*
 * Communication.c
 *
 * Created: 2025-03-25 17:21:21
 *  Author: albin
 */ 

#include <avr/io.h>
#include "Communication.h"

int transmit(Communication *self, int data){
	while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}