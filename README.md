barwin-arduino
==============
Arduino code for Barwin (former codename evobot) - see http://barwin.suuf.cc/ for more info.
This code controls the servos and the scale and communicates with the PC using a serial link.

Dependencies
============
Debian:
	sudo apt-get install arduino python-pip picocom
	sudo pip install ino pyserial

Fedora:
    sudo yum install arduino python-pip picocom
    sudo pip install ino pyserial
    
Compile + Upload + Connect to serial
====================================
	make

See Makefile for details. Tested with Arduino Duemilanove and Arduino Leonardo.

Serial Interface
=====================
Timeout for one command: 50 milliseconds (see SERIAL_TIMEOUT in config.h)
Terminated using space and \r\n

Serial -> Arduino:
------------------
Messages received by Arduino. These messages might be called "commands". Note that
depending on the current state, only few commands might be available and you will
receive an "INVALID_CMD" error if you send other commands. The state diagram might
help to figure out when to send which command.

<dl>
    <dt>POUR x1 x2 x3 ... x_n</dt>
    <dd>pour x_i grams of ingredient i, for i=1..n; will skip bottle if x_n &lt; UPRIGHT_OFFSET</dd>
    <dt>ABORT</dt>
    <dd>abort current cocktail</dd>
    <dt>RESUME</dt>
    <dd>resume after BOTTLE_EMPTY error, use this command when bottle is refilled</dd>
    <dt>DANCE</dt>
    <dd>let the bottles dance!</dd>
    <dt>TARE</dt>
    <dd>
        sets scale to 0, make sure nothing is on scale when sending this command
        <b>Note:</b> taring is deleled, when Arduino is reseted (e.g. on lost serial connection)
    </dd>
    <dt>TURN bottle_nr microseconds</dt>
    <dd>turns a bottle (numbered from 0 to 6) to a position given in microseconds</dd>
    <dt>ECHO</dt>
    <dd>
         Example: ECHO ENJOY\r\n
         Arduino will then print "ENJOY"
         This is a workaround to resend garbled messages manually.
         see also: https://github.com/rfjakob/barwin-arduino/issues/5
    </dd>
    <dt>NOP</dt>
    <dd>
        Arduino will do nothing and send message "DOING_NOTHING".
        This is a dummy message, for testing only.
    </dd>
</dl>

Arduino -> Serial:
------------------
Messages sent by Arduino. These messages are status messages (or replies to commands).

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
    <dd>sent after a successfully mixed cocktail, not sent if poured amount 
    is not accurate enough (insteadan ERROR POURING_INACCURATE will be sent</dd>
    <dt>ERROR error_desc params</dt>
    <dd>
    	<dl>
    		<dt>error_desc: str</dt>
            <dd>
                For the full list of Strings see <a href=lib/errors/errors.cpp>errors.cpp</a>. Important examples:
                <ul>
                    <li>CUP_GONE</li>
                    <li>BOTTLE_EMPTY</li>
                    <li>INVAL_CMD</li>
                    <li>...</li>
                </ul>
            </dd>
        </dl>
    </dd>
    <dt>NOP</dt>
    <dd>
        If Arduino gets command NOP, it replies with NOP and does nothing.
    </dd>

</dl>

State Diagram
=============
Can be rendered via http://yuml.me/diagram/plain/class/draw

    [READY]-POUR>[POURING]
    [READY]-POUR>[WAITING_FOR_CUP]
    [WAITING_FOR_CUP]->[ERROR CUP_TO]
    [ERROR CUP_TO]->[READY]
    [WAITING_FOR_CUP]-cup placed on scale>[POURING]
    [POURING]-next bottle>[POURING]
    [POURING]-finished pouring>[ENJOY]
    [POURING]-finished pouring but inaccurate>[ERROR POURING_INACCURATE]
    [ERROR POURING_INACCURATE]-take cup>[READY]
    [POURING]-ABORT>[READY]
    [POURING]->[ERROR other_error]
    [ERROR other_error]->[READY]
    [ENJOY]-take cup>[READY]
    [READY]->[READY]
    [POURING]->[ERROR CUP_GONE]
    [POURING]->[ERROR BOTTLE_EMPTY]
    [ERROR BOTTLE_EMPTY]-RESUME>[POURING]
    [ERROR CUP_GONE]->[WAITING_FOR_CUP]

Pre-rendered PNG: http://yuml.me/662bff34 (created 2014-05-04, may be out of date)

Further Documentation
=====================
* Scale pinout diagram
* Eagle Schematics (TODO)
* Eagle Layout (TODO)
In the Documentation folder


Useful links
============
* ino quickstart http://inotool.org/quickstart
* ADS1231 datasheet http://www.ti.com/lit/ds/symlink/ads1231.pdf
