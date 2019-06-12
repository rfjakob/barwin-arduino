avrdude -C//etc/avrdude.conf -v -patmega2560 -cstk500v2 -P/dev/ttyACM0 -b115200 -D -Uflash:w:$PWD/arduino-builder/barwin-arduino.ino.hex:i
