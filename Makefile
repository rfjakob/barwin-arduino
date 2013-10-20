all: build upload

build:
	ino build -m leonardo

upload:
	ino upload -m leonardo

clean:
	rm -Rf .build
