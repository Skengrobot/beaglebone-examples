# Makefile test for cross compiling on beaglebone
# Building and  debugging remotely with qtcreator

CC = /usr/local/angstrom/arm/bin/arm-angstrom-linux-gnueabi-gcc
CFLAGS += -Wall -pthread

TARGET = sysfs-hall-timer 

all: $(TARGET)

$(TARGET): main.cpp hallTimer.o 
	$(CC) $(CFLAGS) -o $@ main.cpp

hallTimer.o: hallTimer.cpp hallTimer.h
	$(CC) $(CFLAGS) -c hallTimer.cpp

clean:
	rm *.o $(TARGET)
