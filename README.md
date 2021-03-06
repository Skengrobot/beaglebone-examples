beaglebone-examples
===================

Example programs for the BeagleBone and BeagleBone Black. I've written most of this code for other 
projects but I'll release things as write them because there aren't many resources out there.
I've tried to comment things and make the code easy to read.


Makefile cross-compilation for BeagleBone
=========================================
To compile code for the BeagleBone you need to download the Angstrom Linux toolchain (make sure
you get the armv7 version that fits the architecture of the computer you're compiling on and get
the version that ships with the QT SDK). You can get that here:

http://www.angstrom-distribution.org/toolchains/

To isntall it run:

	sudo tar -C / -xjvf angstrom-2011.03-x86_64-linux-armv7a-linux-gnueabi-toolchain-qte-4.6.3.tar.bz2

You can use the makefile that I uploaded with the example to compile anything and then you have to
upload the executable with scp or sftp.
Now you should be able to build code with `make` and loading them to the BeagleBone with scp.

Updating car-main
=================
Currently the all the telemetry is running in one process called car-main in /home/root/car-main/.
This runs as systemd service when you boot the Bone. If you made changes and you want to test this
running:

	scp car-main *.o root@176.192.7.2:car-main/

won't work because the system is using these files. You need to stop the service first. Run:

	systemctl disable car-main.service && reboot

and you should be able to change the files. If you're testing repeatedly and you don't want to 
enable the service and reboot every time you can just start the program normally:

	/home/root/car-main/car-main

When you're done run
	
	systemctl enable car-main.service && systemctl start car-main.service

and it'll work next time you boot.

Configuring CANBus
==================
To enable CANBus you need to load all the drivers

	modprobe can can-dev can-raw

You can check the kenel messages to see if that worked. Now you need to configure the network
interface. Most usual CAN configurations can be set by bitrate, but in my experience it's
better to set this up with CAN-specific parameters. The tq stands for "time quanta" and is in
nanoseconds, parameters are units of tq. Check the kernel documentation and CANBus wiki for
more details. Run:

	ip link set can0 type can tq 125 prop-seg 6 phase-seg1 7 phase-seg2 2 sjw 1 restart-ms 100

The restart-ms is useful because if the bus is swamped with errors and nothing else can transmit,
it'll clear everything after that time elapses. You then need to start the interface:

	ifconfig can0 up
