#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include "ThreadStruct.h"
#include "Gui.h"
#include "Cummunication.h"

pthread_mutex_t guiMutex; // Lär nog inte behövas
pthread_t carRemover, theRunner;
int northQueue = 0, northLight = 0, onBridge = 0, southLight = 0, southQueue = 0;

void *sleepDeprived(void *t){
    Gui *sim = (Gui *)t;
    sleep(5);
    pthread_mutex_lock(&guiMutex);
    if (sim->onBridge != 0){
        sim->onBridge--;
    }
    pthread_mutex_unlock(&guiMutex);
    printMe(sim);
    pthread_exit(NULL);
}

void printMe(Gui *sim){
    pthread_mutex_lock(&guiMutex);
    //on = inputRead(ts->serial_port, sim);
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
}

int inputRead(int serial_port, Gui *sim) {
    uint8_t SOUTH = 0b0100;
    uint8_t NORTH = 0b0001;
    /*Thread *ts = (Thread*)t;
    Gui *sim = (Gui *)ts->Sim;*/
    pthread_mutex_lock(&guiMutex);
    char input;
    scanf("%c", &input);
    switch (input) {
        case 'n': 
            sim->northQueue++;
            write(serial_port, &NORTH, 1);
            break;
        case 's':
            sim->southQueue++;
            write(serial_port, &SOUTH, 1);
            break;
        case 'e':
            printf("Bridge collapsed...");
            return 0;
            /*close(ts->serial_port);
            exit(0);*/
            
        default:
            break;
    }
    pthread_mutex_unlock(&guiMutex);
    printMe(sim);
    return 1;
}

void menu(void *t) {
    int on = 1;
    Thread *ts = (Thread*)t;
    Gui *sim = (Gui *)ts->Sim;
    while (on) {
        on = inputRead(ts->serial_port, sim);
        //printMe(sim);
        sleep(0.01);
    }
}

// trust
void theProcess(int serial_port, Gui *sim) {
    uint8_t NORTHDEQ = 0b0010;
    uint8_t SOUTHDEQ = 0b1000;
    /*Thread *ts = (Thread*) t;
    Gui *sim = (Gui*)ts->Sim;*/
    pthread_mutex_lock(&guiMutex);
    if (sim->northLight && !(sim->southLight) && sim->northQueue) {
        sim->northQueue--;
        sim->onBridge++;
        //sleep(1);       // Drive on delay
        write(serial_port, &NORTHDEQ, 1);
    }
    if (!(sim->northLight) && (sim->southLight) && sim->southQueue) {
        sim->southQueue--;
        sim->onBridge++;
        //sleep(1);       // Drive on delay
        write(serial_port, &SOUTHDEQ, 1);
    }
    pthread_mutex_unlock(&guiMutex);
    printMe(sim);
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
    printMe(sim);
    theProcess(ts->serial_port, sim);
}

/*
Om bil samma riktning {
    delay 1 sekund
}

borde funka varje gång 
*/