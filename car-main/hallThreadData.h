/*********************************************************************
          hallThreadData.h (c) Rudi Plesch 2013

A struct for passing all data relevant to one hall effect sensor
thread into it at once

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


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef _HTdata_H_
#define _HTdata_H_

struct hallThreadData {
    float speed;
    int isFront;
    int pinFileDescriptor;

    pthread_mutex_t theMutex;
};

#endif
