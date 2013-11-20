

 - [ ] Protothreads?
 - [ ] when cup is not there while pouring, abort pouring procedure (check if weight gets less between bottles)
 - [ ] fix Makefile for Arduino One
 - [ ] serial reconnect --> reset arduino?
 - [ ] problems with pin 13
 - [ ] do we check the number of params?
 - [ ] Serial command: calibrate scale, SET_OFFSET (tare)
 - [ ] Serial command: all to pos_up
 - [ ] auto calibration for UPRIGHT_OFFSET
 - [ ] abort button
 - [ ] abort command
 - [ ] Error handling? (scale?)
 - [ ] READY: add 0/1 if cup or not
 - [ ] Serial commands for different speed?
 - [ ] UPRIGHT_OFFSET / 2.0 (if less, don't do anything if more pour)
 - [ ] dancing bottles!

 
 - a better way to use something like threads? service interrupt?
        http://playground.arduino.cc/Deutsch/HalloWeltMitInterruptUndTimerlibrary
        http://letsmakerobots.com/node/28278
 - Protothreads:
    https://github.com/daijo/ArduinoLibraries/blob/master/ProtoThreads/examples/pt_led_example/pt_led_example.pde
    http://webcache.googleusercontent.com/search?q=cache:G4uLcJU7Q_QJ:dunkels.com/adam/pt/expansion.html+&cd=4&hl=de&ct=clnk&client=ubuntu
    https://github.com/benhoyt/protothreads-cpp
