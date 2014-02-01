IS_LEONARDO:=$(shell [ -e /dev/ttyACM0 ] && echo 1)
ifdef IS_LEONARDO
	BOARD=leonardo
	BAUDRATE=115200
	TTY=/dev/ttyACM
else
	BOARD=atmega328
	BAUDRATE=9600
	TTY=/dev/ttyUSB
endif

#BOARD=uno
#TTY=/dev/ttyACM

all: build size upload serial

size:
	avr-size .build/*/firmware.elf

build:
	ino build -m $(BOARD)

upload:
	ino upload -m $(BOARD)

serial:
	sleep 2s # /dev/ttyACMx needs some time to appear
	echo "NOTE: ctrl-ax to exit picocom"
	mkdir -p log
	picocom -b $(BAUDRATE) $(TTY)? | tee -a log/serial-`date --iso`.log
	# if above does not work, use instead:
	#ino serial

clean:
	rm -Rf .build
