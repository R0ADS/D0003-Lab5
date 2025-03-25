/*
 * Handler.c
 *
 * Created: 2025-03-25 17:49:49
 *  Author: albin
 */ 

#include "TinyTimber.h"
#include "Handler.h"

void updateDisplay(Handler *self) {
	ASYNC(self->lcd, printAt, self->northQueue * 10);
	ASYNC(self->lcd, printAt, self->onBridge * 10 + 2);
	ASYNC(self->lcd, printAt, self->southQueue * 10 + 4);
}

void readValue(Handler *self) {
	int value = ASYNC(self->com, receive, NULL);
	if (value && 1) {			// If a car enters northQueue
		self->northQueue++;
	}
	if (value && 2) {			// Car enters north
		self->northQueue--;
		self->onBridge++;
	}
	if (value && 4) {
		self->southQueue++;
	}
	if (value && 8) {
		self->southQueue--;
		self->onBridge++;
	}
	ASYNC(self, updateDisplay, NULL);
}


void Switcher (Handler *self) {
	if (self->isNorth) {
		if (self->northQueue){
			ASYNC(self->com, transmit, 0b1001);		// Green north
		}
		else if (self->southQueue && self->onBridge == 0){
			self->isNorth = 0;
			ASYNC(self->com, transmit, 0b0110);		// Green south
		}
	}
	else {
		if (self->southQueue){
			ASYNC(self->com, transmit, 0b0110);		// Green south
		}
		else if (self->northQueue && self->onBridge == 0){
			self->isNorth = 1;
			ASYNC(self->com, transmit, 0b1001);		// Green north
		}
	}

}