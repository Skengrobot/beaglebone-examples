/*********************************************************************
          transmitter.c (c) Rudi Plesch 2013

Function function that uses /dev/TTYO4 to interface to an XBEE
for telemetery. The port is hard coded for now.

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

*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#define usartPort "/dev/ttyO4"

void* transmitterFunction(void* data) {

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    FILE* usartFile;
    usartFile = fopen(usartPort, "w");

    if (!usartFile)
        printf("Could not open USART port file\n");

    while (1) {
        fprintf(usartFile,"21,13,54,35,13,43,41,34,13,4,43,54,67,8,6,51,2,3\n");
        usleep(1000000);
    }

}
