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

int main(void)
{
	int southQueue = 0;
	int northQueue = 0;
	int southBridge = 0;
	int northBridge = 0;
	int isNorth = 1;
	
    Init();
	Lcd lcd = initLcd();
	Communication com = initCommuncation();
	Handler handler = initHandler(&com, &display);
	
}

