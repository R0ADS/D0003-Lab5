/*
 * D0003E-Lab_5.c
 *
 * Created: 2025-03-24 16:43:50
 * Author : anton
 */ 

#include <avr/io.h>
#define FOSC 8000000
#define BAUD 9600
#define MYUBRR ((FOSC/(16UL*BAUD))-1)

void init(void){
	/* Set baud rate */
	UBRR0H = (uint8_t)(MYUBRR<<8);
	UBRR0L = (uint8_t)(MYUBRR);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);

}

uint8_t USART_Receiver(void){
	
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Return said data from buffer */
	return UDR0;
}

uint8_t USART_Transmit(uint8_t data){
	while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

int main(void)
{
	int dicks = 1;
	init();

    while (dicks) 
    {
		uint8_t temptational = USART_Receiver();
		USART_Transmit(temptational);
    }
}

