IS_LEONARDO:=$(shell [ -e /dev/ttyACM ] && echo 1)
ifdef IS_LEONARDO
	BOARD=leonardo
	BAUDRATE=115200
	TTY=/dev/ttyACM
else
	BOARD=atmega328
	BAUDRATE=9600
	TTY=/dev/ttyUSB
endif


all: build upload serial

build:
	ino build -m $(BOARD)

upload:
	ino upload -m $(BOARD)

serial:
	sleep 2s # /dev/ttyACMx needs some time to appear
	echo "NOTE: ctrl-ax to exit picocom"
	picocom -b $(BAUDRATE) $(TTY)? | tee -a serial-`date --iso`.log
	# if above does not work, use instead:
	#ino serial

clean:
	rm -Rf .build
