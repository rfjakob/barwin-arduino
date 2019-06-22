ARDUINO_DIR := $(shell find /opt -maxdepth 1 -type d -name "arduino-*" | tail -1)
$(info Using ARDUINO_DIR=$(ARDUINO_DIR))

all: build upload


.PHONY: build
build:
	mkdir -p arduino-builder
	$(ARDUINO_DIR)/arduino-builder -libraries $(ARDUINO_DIR)/libraries -hardware $(ARDUINO_DIR)/hardware \
		-tools $(ARDUINO_DIR)/hardware/tools -tools $(ARDUINO_DIR)/tools-builder -fqbn arduino:avr:mega:cpu=atmega2560 \
	-build-path arduino-builder \
	barwin-arduino.ino

upload:
	./upload.sh

.PHONY: clean
clean:
	rm -Rf arduino-builder
