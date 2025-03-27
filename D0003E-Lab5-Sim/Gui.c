#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include "ThreadStruct.h"
#include "Gui.h"
#include "Cummunication.h"

pthread_mutex_t guiMutex; // Lär nog inte behövas
pthread_t carRemover;

void *sleepDeprived(void *t){
    Gui *sim = (Gui *)t;
    sleep(5);
    pthread_mutex_lock(&guiMutex);
    sim->onBridge--;
    pthread_mutex_unlock(&guiMutex);
    pthread_exit(NULL);
}

int inputRead(void *t) {
    Thread *ts = (Thread*)t;
    Gui *sim = (Gui *)ts->Sim;
    char input;
    scanf("%c", &input);
    switch (input) {
        case 'n': 
            sim->northQueue++;
            USART_Transmiter((void *)&(ts->serial_port),(uint8_t) 0b0001);
            break;
        case 's':
            sim->southQueue++;
            USART_Transmiter((void *)&(ts->serial_port),(uint8_t) 0b0100);
            break;
        case 'e':
            printf("Bridge collapsed...");
            return 0;
        default:
            break;
    }
    return 1;
}

void menu(void *t) {
    int on = 1;
    Thread *ts = (Thread*)t;
    Gui *sim = (Gui *)ts->Sim;
    int northQueue = 0, northLight = 0, onBridge = 0, southLight = 0, southQueue = 0;
    while (on) {
        pthread_mutex_lock(&guiMutex);
        on = inputRead(&ts);
        if (northQueue != sim->northQueue || 
            northLight != sim->northLight || 
            onBridge != sim->onBridge     || 
            southLight != sim->southLight || 
            southQueue != sim-> southQueue) {
            northQueue = sim->northQueue;
            northLight = sim->northLight;
            onBridge = sim->onBridge;
            southLight = sim->southLight;
            southQueue = sim-> southQueue;
            printf("%d   %d   %d   %d   %d  \n", sim->northQueue,  sim->northLight, sim->onBridge, sim->southLight, sim->southQueue); 
        }
        pthread_mutex_unlock(&guiMutex);
        sleep(0.1);
    }
}

// trust
void theProcess(void *t) {
    Thread *ts = (Thread*) t;
    Gui *sim = (Gui*)ts->Sim;
    pthread_mutex_lock(&guiMutex);
    if (sim->northLight && !(sim->southLight)) {
        sim->northQueue--;
        USART_Transmiter((void *)&(ts->serial_port), 0b0010);
    }
    else if (!(sim->northLight) && sim->southLight) {
        sim->southQueue--;
        USART_Transmiter((void *)&(ts->serial_port), 0b1000);
    }
    sim->onBridge++;
    pthread_mutex_unlock(&guiMutex);
    pthread_create(&carRemover, NULL, sleepDeprived, sim);
}

void handleInput(void *t, uint8_t data) {
    Thread *ts = (Thread*) t;
    Gui *sim = (Gui *)ts->Sim;
    pthread_mutex_lock(&guiMutex);
    if (data & (1 << 0)) {
        sim->northLight = 1;
    }
    else {
        sim->northLight = 0;
    }
    if (data & (1 << 2)) {
        sim->southLight = 1;
    }
    else {
        sim->southLight = 0;
    }
    pthread_mutex_unlock(&guiMutex);
    theProcess(&t);
}

