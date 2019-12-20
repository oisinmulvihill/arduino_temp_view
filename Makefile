# I'm using Elegoo mega 2560 on OSX. I pefer to use command line with Sublime.
#
# To install command line on OSX:
#
# 	brew install arduino-cli
#
# The docs for arduino-cli are located here:
# - https://github.com/arduino/arduino-cli
#
# Oisin Mulvihill
# 2019-11-30
#
PORT=/dev/cu.usbmodemFA241
CORE=arduino:avr
BOARD=${CORE}:mega
PROJECT=temp_view

.PHONY: clean setup compile upload all

# remove the compiled binaries to give a clean start.
clean:
	rm -f ${PROJECT}/*arduino*

# following set up from https://github.com/arduino/arduino-cli
setup:
	arduino-cli core update-index
	arduino-cli board list
	arduino-cli core install ${CORE}
	arduino-cli lib install "DHT sensor library for ESPx"

compile:
	arduino-cli compile --fqbn ${BOARD} ${PROJECT}

upload:
	arduino-cli upload -p ${PORT} --fqbn ${BOARD} ${PROJECT}

all: clean compile upload
