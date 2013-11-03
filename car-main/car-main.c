/*********************************************************************
          car-main.c (c)  Rudi Plesch 2013

    Main program for running car data logging and telemetry. This
calls a POSIX tread for each hall sensor, one for logging, one for
talking to the CAN components,and one for XBee transmission.

* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
* MA 02110-1301, USA.
*
* *********************************************************************/





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

#include "wheel-timer.h"
#include "transmitter.h"

// Transmitter thread declaration
pthread_t transmitterThread;


int main (void) {

    // Created logger and transmitter threads
    pthread_create(&transmitterThread, NULL, transmitterFunction, NULL);

    while (1) {
        ;
    }

    // Send signal to terminate threads
    pthread_cancel(transmitterThread);
    // Wait for them to terminate
    pthread_join(transmitterThread, NULL);

    return 0;
}
