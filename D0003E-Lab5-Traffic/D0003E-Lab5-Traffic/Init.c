/*
 * CFile1.c
 *
 * Created: 2025-02-26 13:09:09
 *  Author: anton
 */ 
#include <avr/io.h>
#include "Init.h"
#define FOSC 8000000
#define BAUD 9600
#define MYUBRR ((FOSC/(16UL*BAUD))-1)

void Init(void) {
	CLKPR = 0x80;    // Fix clock speed
	CLKPR = 0x00;
	
	// LCD init
	LCDCCR = (1 << LCDCC3) | (1 << LCDCC2) | (1 << LCDCC1) | (1 << LCDCC0);
	//       (                      Voltage                              )
	LCDCRB = (1 << LCDCS) | (1 << LCDMUX1) | (1 << LCDMUX0) | (1 << LCDPM2) | (1 << LCDPM1) | (1 << LCDPM0);
	//       (asynch ext)   (        duty cycle           )   (                  segments                 )
	LCDFRR = (1 << LCDCD2) | (1 << LCDCD1) | (1 << LCDCD0);
	//       (            Delen med D                    )
	LCDCRA = (1 << LCDAB) | (1 << LCDEN);
	//       (low power )   (enable lcd)
	
	// ------------------------- Stuffs for communication -------------------------------------------------
	
	/* Set baud rate */
	UBRR0H = (uint8_t)(MYUBRR<<8);
	UBRR0L = (uint8_t)(MYUBRR);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);

}