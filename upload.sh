avrdude -C//etc/avrdude.conf -v -patmega32u4 -carduino -P/dev/ttyACM0 -b9600 -D -Uflash:w:/tmp/arduino_build_766618/bla.ino.hex:i
