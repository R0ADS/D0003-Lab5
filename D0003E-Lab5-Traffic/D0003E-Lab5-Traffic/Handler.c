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

int deQueuer(Handler *self){
	if (self->isNorth){
		self->northQueue--;
		self->onBridge++;
		ASYNC(self, updateDisplay, NULL);
	}
	else{
		self->southQueue--;
		self->onBridge++;
		ASYNC(self, updateDisplay, NULL);
	}
	AFTER(MSEC(5000), self, reduceBridge, NULL);
}

int readValue(Handler *self, int value) {
	if (value == 1) {			// If a car enters northQueue
		self->northQueue++;
	}
	if (value == 2) {			// Car enters north
		self->counter++;
		ASYNC(self, deQueuer, NULL);
	}
	if (value == 4) {
		self->southQueue++;
	}
	if (value == 8) {
		self->counter++;		
		ASYNC(self, deQueuer, NULL);
	}
	ASYNC(self, updateDisplay, NULL);
	ASYNC(self, switcher, NULL);
}

int switcher (Handler *self) {
	if (self->isNorth) {
		if (self->northQueue && self->counter < 5) {			// I added the check with (if other queue > 5 switch queue if bridge is currently empty)
			ASYNC(self->com, transmit, 0b1001);		// Green north
		}
		else {
			if (self->southQueue && self->onBridge == 0) { //why is 2 = 0?!?!
				self->counter = 0;
				self->isNorth = 0;
				ASYNC(self->com, transmit, 0b0110);		// Green south
			}
			if (!(self->southQueue) && self->counter >= 5 && self->onBridge < 5 /*&& self->northQueue*/){
				self->counter--;
				ASYNC(self->com, transmit, 0b1001);		// Green north
			}
			else {
				if (test) {
					ABORT(test);
				}
				test = AFTER(MSEC(1000), self, switcher, NULL);
			}
		}
	}
	else if (!(self->isNorth)){
		if (self->southQueue && self->counter < 5) {
			ASYNC(self->com, transmit, 0b0110);		// Green south
		}
		else {
			if (self->northQueue && self->onBridge == 0) {
				self->counter = 0;
				self->isNorth = 1;
				ASYNC(self->com, transmit, 0b1001);		// Green north
			}
			if (!(self->northQueue) && self->counter >= 5 && self->onBridge < 5 /*&& self->southQueue*/){
				self->counter--;
				ASYNC(self->com, transmit, 0b0110);		// Green south
			}
			else {
				if (test) {
					ABORT(test);
				}
				test = AFTER(MSEC(1000), self, switcher, NULL);
			}
		}
	}
}
