
 - [ ] Problem: orig might be measured if there is no cup
 - [ ] problem: when cup is away, bottle pours directly to scale and causes weight
 - [ ] Error handling? (scale?)
 - [ ] when cup is not there while pouring, abort pouring procedure (check if weight gets less between bottles)
 - [x] empty bottle error handling
 - [x] MAX_DRINK_SIZE implement!
 - [x] rename UPRIGHT_OFFSET --> STOP_EARLY_GRAMS <-- won't fix
 - [ ] auto calibration for UPRIGHT_OFFSET
 - [ ] Serial commands for different speed?
 - [ ] move bottle according to non-linear predefined function
 - [ ] Serial command: calibrate scale, SET_OFFSET (tare)
 - [ ] Serial command: all to pos_up, all down
 - [ ] abort button
 - [ ] abort command
 - [ ] pause/command switch
 - [ ] do we check the number of params?
 - [x] move bottles at the same time?
 - [ ] Protothreads?
 - [ ] fix Makefile for Arduino One
 - [ ] serial reconnect --> reset arduino?
 - [ ] problems with pin 13
 - [x] READY: add 0/1 if cup or not
 - [x] UPRIGHT_OFFSET / 2.0 (if less, don't do anything if more pour)
 - [x] dancing bottles!
 - [x] improve dancing bottles by turning more bottles at the same time

Testing:
    - empty bottle
    - cup away: between pouring, before, after
    - max drink size
    - scale timeout
    - invalid commands

 
 - a better way to use something like threads? service interrupt?
        http://playground.arduino.cc/Deutsch/HalloWeltMitInterruptUndTimerlibrary
        http://letsmakerobots.com/node/28278
 - Protothreads:
    https://github.com/daijo/ArduinoLibraries/blob/master/ProtoThreads/examples/pt_led_example/pt_led_example.pde
    http://webcache.googleusercontent.com/search?q=cache:G4uLcJU7Q_QJ:dunkels.com/adam/pt/expansion.html+&cd=4&hl=de&ct=clnk&client=ubuntu
    https://github.com/benhoyt/protothreads-cpp
