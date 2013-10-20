all: build upload serial

build:
	ino build -m leonardo

upload:
	ino upload -m leonardo

serial:
	sleep 1s # /dev/ttyACMx needs some time to appear
	echo "NOTE: ctrl-ax to exit picocom"
	picocom -b 9600 /dev/ttyACM?

clean:
	rm -Rf .build
