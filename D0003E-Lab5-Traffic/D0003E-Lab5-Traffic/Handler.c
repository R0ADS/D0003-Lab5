/*
 * Handler.c
 *
 * Created: 2025-03-25 17:49:49
 *  Author: albin
 */ 

#include "TinyTimber.h"
#include "Handler.h"
#include <avr/io.h>
#include <stdio.h>



Msg test;
Msg teste;

int Temp(Handler *self, int value) {
	AFTER(MSEC(1000), self, readValue, value);
}

// Displays our three variables on corresponding segments
int updateDisplay(Handler *self) {
	ASYNC(self->lcd, printAt, self->northQueue * 10);
	ASYNC(self->lcd, printAt, self->onBridge * 10 + 2);
	ASYNC(self->lcd, printAt, self->southQueue * 10 + 4);
}

// Seperate function that reduces onbridge (needed for AFTER)
int reduceBridge(Handler *self) {
	self->onBridge--;
	ASYNC(self, updateDisplay, NULL);
}

// Moves car from queue (dependent on dir) to bridge
int deQueuer(Handler *self, int dir){
	if (dir){
		self->northQueue--;
	}
	else{
		self->southQueue--;
	}
	self->onBridge++;
	AFTER(MSEC(1000), self, switcher, NULL);
	//ASYNC(self, updateDisplay, NULL);	// <-- Unnecessary line? Should remove and test
	AFTER(MSEC(5000), self, reduceBridge, NULL);
}

// Reads incoming data and changes variables accordingly
int readValue(Handler *self, int value) {
	if (value == 1) {			// Car enters northQueue
		self->northQueue++;
		ASYNC(self, switcher, NULL);
	}
	if (value == 2) {			// Car enters bridge from north
		self->counter++;
		ASYNC(self, deQueuer, 1);
	}
	if (value == 4) {			// Car enters southQueue
		self->southQueue++;
		ASYNC(self, switcher, NULL);
	}
	if (value == 8) {			// Car enters bridge from south
		self->counter++;		
		ASYNC(self, deQueuer, 0);
	}
	ASYNC(self, updateDisplay, NULL);
	//ASYNC(self, switcher, NULL);
}

// Determines how and when to signal green on either side
/*int switcher (Handler *self) {
	if (self->isNorth) {
		if (self->northQueue && self->counter < 5) {
			ASYNC(self->com, transmit, 0b1001);				// Green north
		}
		else {
			if (self->southQueue && self->onBridge == 0) {
				self->counter = 0;
				self->isNorth = 0;
				ASYNC(self->com, transmit, 0b0110);			// Green south
			}
			if (!(self->southQueue) && self->counter >= 5 && self->onBridge < 5) {
				self->counter--;
				ASYNC(self->com, transmit, 0b1001);			// Green north
			}
			if (self->southQueue && !(self->northQueue) && self->counter >= 5 && !(self->onBridge == 0)){
				if (test) {
					ABORT(test);
				}
				test = AFTER(MSEC(1250), self, switcher, NULL);	// NOTE: time needs to be above 1 second to be sure that the update has happend
			}
			
			else {
				if (test) {
					ABORT(test);
				}
				test = AFTER(MSEC(1250), self, switcher, NULL);	// NOTE: time needs to be above 1 second to be sure that the update has happend
			}
		}
	}
	else if (!(self->isNorth)){
		if (self->southQueue && self->counter < 5) {
			ASYNC(self->com, transmit, 0b0110);				// Green south
		}
		else {
			if (self->northQueue && self->onBridge == 0) {
				self->counter = 0;
				self->isNorth = 1;
				ASYNC(self->com, transmit, 0b1001);			// Green north 
			}
			if (!(self->northQueue) && self->counter >= 5 && self->onBridge < 5) {
				self->counter--;
				ASYNC(self->com, transmit, 0b0110);			// Green south
			}
			if (self->northQueue && !(self->southQueue) && self->counter >= 5 && !(self->onBridge == 0)){
				if (test) {
					ABORT(test);
				}
				test = AFTER(MSEC(1250), self, switcher, NULL);	// NOTE: time needs to be above 1 second to be sure that the update has happend
			}
			else {
				if (test) {
					ABORT(test);
				}
				test = AFTER(MSEC(1250), self, switcher, NULL);	// NOTE: time needs to be above 1 second to be sure that the update has happend
			}
		}
	}
}*/

// Determines how and when to signal green on either side
// Determines how and when to signal green on either side
int switcher (Handler *self) {
	if (test){
		ABORT(test);
		test = NULL;
	}
    if (self->isNorth) {
        if (self->northQueue && self->counter < 5) {    // Let trhrough
            ASYNC(self->com, transmit, 0b1001);
        }
        else if (self->counter >= 5) {
            if (self->onBridge == 0) {
                self->counter = 0;
                if (self->southQueue) {                    // Switch side
                    self->isNorth = 0;
                    ASYNC(self->com, transmit, 0b0110);
                }
                else {
                    ASYNC(self->com, transmit, 0b1001);    // Reset and keep
                }
            }
            else {												// !Delay
                test = AFTER(MSEC(1250), self, switcher, NULL);
            }
        }
        else if (!(self->northQueue) && self->onBridge == 0) {
            self->counter = 0;
            self->isNorth = 0;
            ASYNC(self->com, transmit, 0b0110);
        }
    }
    else  {
        if (self->southQueue && self->counter < 5) {
            ASYNC(self->com, transmit, 0b0110);
        }
        else if (self->counter >= 5) {
            if (self->onBridge == 0) {
                self->counter = 0;
                if (self->northQueue) {
                    self->isNorth = 1;
                    ASYNC(self->com, transmit, 0b1001);
                }
                else {
                    ASYNC(self->com, transmit, 0b0110);
                }
            }
            else {												// !Delay
                test = AFTER(MSEC(1250), self, switcher, NULL);
            }
        }
        else if (!(self->southQueue) && self->onBridge == 0){
            self->counter = 0;
            self->isNorth = 1;
            ASYNC(self->com, transmit, 0b1001);
        }
    }
}