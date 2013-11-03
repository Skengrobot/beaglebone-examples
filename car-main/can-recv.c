/*********************************************************************
          can-recv.c (c)  Rudi Plesch 2013

    CAN reciver that communicates with the ADC module and can be
extended to talk to other CAN components. The structure is similar
to what's happening in wheel-timer.c, so you should look at that
to get an idea fo how poll() works.

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

*********************************************************************/
//
//
//            __________
//           /          \
//          / _  _  _  _ \
//         /  \\//  \\//  \
//        /   //\\  //\\   \
//       /                  \
//      /    _____________   \
//     /    |_____________|   \
//   _/                        \_
//  |____________________________|

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <time.h>
#include <sys/time.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/socket.h>

#include <pthread.h>
#include <poll.h>

// Kind of hack, but I can't link linux/socket.h properly,
// where these are defined

#ifndef PF_CAN
#define PF_CAN 29
#endif
#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

#define CAN_TIMEOUT 20000 // 20 seconds

void* canRecv(void* data) {

    int sock; // This works as a file descriptor
    struct sockaddr_can address;
    struct ifreq ifr;

    char readBuffer[64];

    // Structs for polling and timestamps
    struct timeval systemTime;
    struct pollfd socketPoll;
    long unsigned int time;
    int pollReturn;

    if((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        printf("Couldn't open socket\n");
        return -1;
    }

    address.can_family = AF_CAN;
    strcpy(ifr.ifr_name, "can1");
    ioctl(sock, SIOCGIFINDEX, &ifr);
    address.can_ifindex = ifr.ifr_ifindex;

    bind(sock, (struct sockaddr*)&address, sizeof(address));

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    while(1) {
        memset((void*)&socketPoll, 0, sizeof(socketPoll));

        socketPoll.fd = sock;
        socketPoll.events = POLLIN;

        pollReturn = poll(&socketPoll, 1, CAN_TIMEOUT);
        if(pollReturn < 0) {
            printf("CANBus poll failed\n");
            return -1;
        }

        else if(pollReturn == 0) {
            printf("CANBus connection timed out\n");
        }

        else if(socketPoll.revents & POLLIN && pollReturn > 0) {
            recv(sock, readBuffer, sizeof(readBuffer), 0);
        }
    }


    return(1);
}
