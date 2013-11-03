######################################################################
# Makefile for cross-compiling main car program
#
# (c) Rudi Plesch 2013 
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
######################################################################


CC = /usr/local/angstrom/arm/bin/arm-angstrom-linux-gnueabi-gcc
CFLAGS += -Wall -pthread

TARGET = car-main

all: $(TARGET)

$(TARGET): car-main.o transmitter.o can-recv.o
	$(CC) $(CFLAGS) -o $@ car-main.o transmitter.o can-recv.o

car-main.o: car-main.c transmitter.h
	$(CC) $(CFLAGS) -c car-main.c

transmitter.o: transmitter.c transmitter.h
	$(CC) $(CFLAGS) -c transmitter.c

can-recv.o: can-recv.c can-recv.h
	$(CC) $(CFLAGS) -c can-recv.c

clean:
	rm $(TARGET) *.o
