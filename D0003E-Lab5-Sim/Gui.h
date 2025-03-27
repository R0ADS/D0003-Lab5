#ifndef GUI_H_
#define GUI_H_

// includa det som behövs

typedef struct {
    pthread_mutex_t guiMutex;
} Gui;

#define initGui() {}

#endif