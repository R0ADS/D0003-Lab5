/*
 * Communication.c
 *
 * Created: 2025-03-25 17:21:21
 *  Author: albin
 */ 

#include <avr/io.h>
#include "Communication.h"

uint8_t receive(void){
	
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Return said data from buffer */
	return UDR0;
}

uint8_t transmit(uint8_t data){
	while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}