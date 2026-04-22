# Upload Instructions

Uses `arduino-cli` (the legacy `arduino-builder` referenced in the Makefile is no longer available).

## One-time setup

Install the AVR core (if not already installed):

```
arduino-cli core install arduino:avr
```

Install required libraries:

```
arduino-cli lib install "Servo"
arduino-cli lib install "LiquidCrystal"
```

## Compile

```
arduino-cli compile --fqbn arduino:avr:mega:cpu=atmega2560 barwin-arduino.ino
```

## Upload

```
arduino-cli upload --fqbn arduino:avr:mega:cpu=atmega2560 --port /dev/ttyACM0 barwin-arduino.ino
```
