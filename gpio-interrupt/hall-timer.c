/*********************************************************************
          hall-timer.c (c) 2013 Rudi Plesch
 
Sysfs implementation of a hall effect timer that uses the poll()
funciton to wait on a rising edge interrupt on the GPIO pin with the
hall effect sensor.
The poll() is done in a separate POSIX thread and it reads the system
clock.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA  02110-1301, USA.

*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include <math.h>

#include "hallThreadData.h"

#define GPIOdirectory "/sys/class/gpio"
#define TIMEOUT_0_SPEED 250

#define FRONT_CIRCUMFERENCE 1.626 // meters
#define REAR_CIRCUMFERENCE 1.785 // meters

#define MPS_TO_KPH 3.6

// File pointers for sysfs gpio
// They are static so their scope is the entire file
static FILE* fileExport;
static FILE* fileEdge;

// pthread declaration
pthread_t frontLeftThread;

// Instantiations of hallThreadData for each sensor and initialize
struct hallThreadData* frontLeftData;

// Function to execute in the thread
void* timerFunction (void* data);

// Other functions
int gpioSetup (char pin[]);
int cleanUp (struct hallThreadData* dataStruct);

int main (void) {

    // These are the steps to take for each sensor
    frontLeftData = malloc(sizeof(struct hallThreadData)); // Initilize the pointer
    frontLeftData->pinFileDescriptor = gpioSetup("22"); // gpioSetup returns a file descriptor
    frontLeftData->isFront = 1;
    // Create thread, pass in function handle and data struct
    pthread_create (&frontLeftThread, NULL, timerFunction, (void*)frontLeftData);

    while (1) {
        printf("Front left speed is %f km/h\n", frontLeftData->speed);
        usleep(250000);
    }

    // Send signal to terminate thread
    pthread_cancel(frontLeftThread);
    // Wait for it to terminate
    pthread_join (frontLeftThread, NULL);

    cleanUp(frontLeftData);
    return 0;
}

void* timerFunction(void *data) {
    printf("Poll thread is running...\n");

    // casting data pointer
    struct hallThreadData* threadData;
    threadData = (struct hallThreadData*)data;
    // Return values for poll and read, though we'll ignore read
    int pollReturn, readReturn;
    // Buffer to read interrput to, this is only needed to clear the data
    char* buffer[64];

    // Time struct for reading system clock values for ms
    struct timeval systemTime;
    struct pollfd gpioDescriptor;

    unsigned long int lastTime, currentTime;

    // Making the thread cancellable, this will execute during poll() block
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    // While loop to indefinitely look for interrupts
    while (1) {
        // Initializes gpioDescriptor to 0
        memset ((void*)&gpioDescriptor, 0, sizeof(gpioDescriptor));

        // The fd member holds the file pointer
        gpioDescriptor.fd = threadData->pinFileDescriptor;
        // The events member holds the event priority, set to urgent
        gpioDescriptor.events = POLLPRI;

        // Pass in pointer of file to poll, number of files to poll and 1s timeout
        pollReturn = poll (&gpioDescriptor, 1, TIMEOUT_0_SPEED);

        // Check the return value of poll to see if the file is opened properly
        if (pollReturn < 0) {
            printf ("\npoll() failed \n");
            return -1;
        }

        // This is the timeout condition
        else if (pollReturn == 0) {
            threadData->speed = 0;
        }

        // Masks the event return value with the POLLPRI constant
        // These conditions are tautological, but you can scale from here
        else if (gpioDescriptor.revents & POLLPRI && pollReturn > 0) {
            // Need to call read to clear the interrupt data, even though it's not interesting
            readReturn = read (gpioDescriptor.fd, buffer, 64);

            // Calculate elapsed time
            lastTime = currentTime;
            gettimeofday(&systemTime, NULL);
            currentTime = (systemTime.tv_sec) * 1000 + (systemTime.tv_usec) / 1000;

            unsigned long int elapsedMilliSeconds = currentTime - lastTime;
            if (threadData->isFront)
                threadData->speed = 1000 * MPS_TO_KPH * FRONT_CIRCUMFERENCE / elapsedMilliSeconds;
            else
                threadData->speed = 1000 * MPS_TO_KPH * REAR_CIRCUMFERENCE / elapsedMilliSeconds;
            //printf ("GPIO interrupt occurred\n");
            //printf ("Time Elapsed: %lu ms\n", elapsedMilliSeconds);
        }

        fflush (stdout);
    }
    return 0;
}

int gpioSetup (char pin[2]) {
    // This function sets up the pin that the hall timer will be on
    char pinLocation[30];
    strncpy (pinLocation, GPIOdirectory, 16);
    strncat (pinLocation, "/gpio", 5);
    strncat (pinLocation, pin, 2);

    char pinEdge[30];
    strncpy (pinEdge, pinLocation, 23);
    strncat (pinEdge, "/edge", 5);

    char pinExport[30];
    strncpy (pinExport, GPIOdirectory, 16);
    strncat (pinExport, "/export", 7);

    char pinValue[30];
    strncpy (pinValue, pinLocation, 23);
    strncat (pinValue, "/value", 7);

    fileExport = fopen (pinExport, "w");
    // Exports the pin so it can be used in used space
    if (!fileExport) {
        printf("Could not open export file\n");
        return 0;
    }

    fprintf (fileExport,"%s\n",pin);
    fclose (fileExport);

    fileEdge = fopen (pinEdge, "w");
    // Configures a rising edge interrupt on the pin
    if (!fileEdge) {
        printf("Could not open pin edge file\n");
    }

    fprintf (fileEdge, "rising\n");
    fclose (fileEdge);

    // Use open() instead of fopen() to return a file descriptor instead of pointer
    int fileDescriptor = open (pinValue, O_RDONLY | O_NONBLOCK);

    return fileDescriptor;
}

int cleanUp (struct hallThreadData* dataStruct) {
    // Basic cleanup function, this will most likely grow with requirements
    close (dataStruct->speed);
    free (dataStruct);
    return 1;
}
