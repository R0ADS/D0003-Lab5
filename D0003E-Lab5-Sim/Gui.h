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
void theProcess(void *t);
void handleInput(void *t, uint8_t data);

#endif