/*
 * D0003E-Lab5-Traffic.c
 *
 * Created: 2025-03-25 16:58:15
 * Author : albin
 */ 

#include <avr/io.h>
#include "Handler.h"
#include "Init.h"
#include "Communication.h"
#include "Lcd.h"
#include "TinyTimber.h"
#include "InterruptHandler.h"

int main(void) {
    Init();
	Lcd lcd = initLcd();
	Communication com = initCommunication();
	Handler handler = initHandler(&com, &lcd);
	InterruptHandler interruptHandler = initInterruptHandler(&handler);
	INSTALL(&interruptHandler, interrupter, IRQ_USART0_RX);
	return tinytimber(NULL, NULL, NULL);
}

