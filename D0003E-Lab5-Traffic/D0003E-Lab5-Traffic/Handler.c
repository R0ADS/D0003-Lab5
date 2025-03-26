/*
 * Handler.c
 *
 * Created: 2025-03-25 17:49:49
 *  Author: albin
 */ 

#include "TinyTimber.h"
#include "Handler.h"
#include <avr/io.h>

Msg test;
Msg teste;

int updateDisplay(Handler *self) {
	ASYNC(self->lcd, printAt, self->northQueue * 10);
	ASYNC(self->lcd, printAt, self->onBridge * 10 + 2);
	ASYNC(self->lcd, printAt, self->southQueue * 10 + 4);
}

int reduceBridge(Handler *self) {
	self->onBridge--;
	ASYNC(self, updateDisplay, NULL);
}

int readValue(Handler *self, int value) {
	if (value == 1) {			// If a car enters northQueue
		self->northQueue++;
	}
	if (value == 2) {			// Car enters north
		self->northQueue--;
		self->onBridge++;
		AFTER(MSEC(5000), self, reduceBridge, NULL);
	}
	if (value == 4) {
		self->southQueue++;
	}
	if (value == 8) {
		self->southQueue--;
		self->onBridge++;
		AFTER(MSEC(5000), self, reduceBridge, NULL);
	}
	ASYNC(self, updateDisplay, NULL);
	ASYNC(self, switcher, NULL);
}

int switcher (Handler *self) {
	if (self->isNorth) {
		if (self->northQueue && self->counter < 5) {			// I added the check with (if other queue > 5 switch queue if bridge is currently empty)
			self->counter++;
			ASYNC(self->com, transmit, 0b1001);		// Green north
		}
		else {
			if ((self->southQueue && self->onBridge == 0) || (self->onBridge == 0 && self->counter >= 5)) {
				self->counter = 1;
				self->isNorth = 0;
				ASYNC(self->com, transmit, 0b0110);		// Green south
			}
			else {
				if (test) {
					ABORT(test);
				}
				test = AFTER(MSEC(200), self, switcher, NULL);	// Confuse 
			}
		}
	}
	else {
		if (self->southQueue && self->counter < 5) {
			self->counter++;
			ASYNC(self->com, transmit, 0b0110);		// Green south
		}
		else {
			if ((self->northQueue && self->onBridge == 0) || (self->onBridge == 0 && self->counter >= 5)) {
				self->counter = 1;
				self->isNorth = 1;
				ASYNC(self->com, transmit, 0b1001);		// Green north
			}
			else {
				if (test) {
					ABORT(test);
				}
				test = AFTER(MSEC(200), self, switcher, NULL);
			}
		}
	}
}