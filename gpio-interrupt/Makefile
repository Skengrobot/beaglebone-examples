# Makefile test for cross compiling on beaglebone
# Building and  debugging remotely with qtcreator

CC = /usr/local/angstrom/arm/bin/arm-angstrom-linux-gnueabi-gcc
CFLAGS += -Wall -pthread

TARGET = sysfs-hall-timer 

all: $(TARGET)

$(TARGET): hall-timer.c hallThreadData.h
	$(CC) $(CFLAGS) -o $@ hall-timer.c

clean:
	rm *.o $(TARGET)
