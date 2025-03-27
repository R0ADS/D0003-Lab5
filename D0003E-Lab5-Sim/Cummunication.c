#include "ThreadStruct.h"
#include "Gui.h"
#include "Cummunication.h"

void *USART_Receiver(void *t){
	uint8_t recived_byte;
    Thread *ts = (Thread*)t;    // convert void * to point at int *
	while (1) {
        int byte_read = read(ts->serial_port, &recived_byte, 1); // read byte
        if (byte_read > 0 && recived_byte != 0){
            handleInput(t, recived_byte);
        }
    }
    return NULL ;
}

void USART_Transmiter(void *t, uint8_t data) {
    Thread *ts = (Thread*)t;
    write(ts->serial_port, (const void *) &data, 1);
}