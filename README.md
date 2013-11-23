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
    <dt>READY current_weight is_cup_there</dt>
    <dd>
        <dl>
    		<dt>current_weight: int</dt>
    		<dd>current weight on scale in grams</dd>
        	<dt>is_cup_there: int</dt>
        	<dd>0 if no cup, 1 if cup on scale (Arduino assumes cup is there if weight > WEIGHT_EPSILON)</dd>
        </dl>
</dd>
    <dt>ERROR error_desc</dt>
    <dd>
    	<dl>
    		<dt>error_desc: str</dt>
            <dd>
One of the following strings:
 - CUP_TIMEOUT_REACHED
 - INVALID_COMMAND
            </dd>
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
