evobot-arduino
==============
Arduino code for the evobot.
Controls the servos and the scale and communicates with the PC using a serial link.

Dependencies
============
	sudo apt-get install arduino python-pip picocom
	sudo pip install ino pyserial

Compile + Upload + Connect to serial
====================================
	make

See Makefile for details. Tested with Arduino Duemilanove and Arduino Leonardo.

Serial Interface
=====================
TODO: describe this better!

Serial -> A:
    POUR x1 x2 x3 ... x_n

A -> Serial:
    READY
    ERROR error_desc
        CUP_TIMEOUT_REACHED
        INVALID_COMMAND
    WAITING_FOR_CUP
    ENJOY x1 x2 x3 ... x_n


Further Documentation
=====================
In the Documentation folder


Useful links
============
* ino quickstart http://inotool.org/quickstart
* ADS1231 datasheet http://www.ti.com/lit/ds/symlink/ads1231.pdf
