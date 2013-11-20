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

timeout: 100milliseconds
terminating using space and \r\n

Serial -> A:
    POUR x1 x2 x3 ... x_n

A -> Serial:
<dl>
    <dt>READY</dt>
    <dd>TODO</dd>
    <dt>ERROR error_desc</dt>
    <dd>
    	<dl>
    		<dt>CUP_TIMEOUT_REACHED</dt>
    		<dd>TODO</dd>
        	<dt>INVALID_COMMAND</dt>
        	<dd>TODO</dd>
        </dl>
    </dd>
    <dt>WAITING_FOR_CUP</dt>
    <dd>TODO</dd>
    <dt>ENJOY x1 x2 x3 ... x_n</dt>
    <dd>TODO</dd>
</dl>

Further Documentation
=====================
In the Documentation folder


Useful links
============
* ino quickstart http://inotool.org/quickstart
* ADS1231 datasheet http://www.ti.com/lit/ds/symlink/ads1231.pdf
