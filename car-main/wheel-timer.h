/*********************************************************************
          wheel-timer.h written by Rudi Plesch for UBC Eracing

    Declaration for function that configures a rising edge interrput on
a GPIO pin for use with a hall effect sensor. Also seclares a struct
for passing data into a thread as well as pin setup and cleanup
functions.

*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef _WHEELTIMER_H_
#define _WHEELTIMER_H_

struct hallThreadData {
    float speed;
    int isFront;
    int pinFileDescriptor;

    pthread_mutex_t theMutex;
};

void* wheelTimer (void* data);

int hallGPIOSetup (char pin[]);
int hallGPIOCleanup (struct hallThreadData* dataStruct);


#endif
