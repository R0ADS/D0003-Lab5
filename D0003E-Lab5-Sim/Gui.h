#ifndef GUI_H_
#define GUI_H_

#include <stdint.h>
#include "ThreadStruct.h"
// includa det som behövs

typedef struct {
    int northLight;
    int southLight;
    int northQueue;
    int southQueue;
    int onBridge;
} Gui;

#define initGui() {0, 0, 0, 0, 0}

void menu(void *t);
void theProcess(int serial_port, Gui *sim);
int inputRead(int serial_port, Gui *sim);
void handleInput(void *t, uint8_t data);
void printMe(Gui *sim);

#endif