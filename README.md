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

Serial -> Arduino:
------------------
Messages received by Arduino.

<dl>
    <dt>POUR x1 x2 x3 ... x_n</dt>
    <dd>will skip bottle if x_n &lt; UPRIGHT_OFFSET</dd>
    <dt>ABORT</dt>
    <dd>abort current cocktail, currently only during bottle refill</dd>
    <dt>RESUME</dt>
    <dd>resume after bottle refill</dd>
    <dt>PAUSE</dt>
    <dd>not implemented yet, pause after next bottle</dd>
    <dt>SET_SCALE_CALIB weight</dt>
    <dd>not implemented yet</dd>
    <dt>NOTHING</dt>
    <dd>
        Arduino will do nothing and send message "DOING_NOTHING".
        This is a dummy message, for testing only.
    </dd>
</dl>

Arduino -> Serial:
------------------
Messages sent by Arduino.

<dl>
    <dt>READY current_weight is_cup_there</dt>
    <dd>
        <dl>
    		<dt>current_weight: int</dt>
    		<dd>current weight on scale in grams</dd>
        	<dt>is_cup_there: int (0-1)</dt>
        	<dd>0 if no cup, 1 if cup on scale (Arduino assumes cup is there if weight > WEIGHT_EPSILON)</dd>
        </dl>
    </dd>
    <dt>WAITING_FOR_CUP</dt>
    <dd>...if Arduino wants to pour something, but there is no cup.</dd>
    <dt>POURING bottle weight</dt>
    <dd>
        Sent before starting to pour bottle (not for skipped bottles).
        <dl>
    		<dt>bottle: int (0-n)</dt>
            <dd>number of bottle (bottles should be numbered from left to right, starting at 0)</dd>
    		<dt>weight: int</dt>
            <dd>weight of cup before pouring</dd>
        </dl>
    </dd>
    <dt>ENJOY x1 x2 x3 ... x_n</dt>
    <dd>TODO</dd>
    <dt>TURN_BOTTLE bottle microseconds</dt>
    <dd>turn a specific bottle for debugging porpuses</dd>
    <dt>ERROR error_desc</dt>
    <dd>
    	<dl>
    		<dt>error_desc: str</dt>
            <dd>
                One of the following strings:
                <ul>
                    <li>CUP_TIMEOUT_REACHED</li>
                    <li>INVALID_COMMAND</li>
                </ul>
            </dd>
        </dl>
    </dd>
    <dt>DOING_NOTHING</dt>
    <dd>
        If Arduino gets command "NOTHING", it does nothing.
        This is a dummy message, for testing only.
    </dd>

</dl>

Further Documentation
=====================
In the Documentation folder


Useful links
============
* ino quickstart http://inotool.org/quickstart
* ADS1231 datasheet http://www.ti.com/lit/ds/symlink/ads1231.pdf
