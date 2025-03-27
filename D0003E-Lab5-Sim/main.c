#include <pthread.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include "Cummunication.h"
#include "Gui.h"
#include "Init.h"


int main() {
    int serial_port = init();
    Gui gui = initGui();
    Thread thread = initThreadArgs(serial_port, &gui);
    pthread_t t1;
    pthread_create(&t1, NULL, USART_Receiver, &thread);

    menu(&thread);     //Infinite loop until "e"

    close(serial_port);
    return 0;
}