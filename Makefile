.ONESHELL:

TARGET=rpi_pico

CMAKE_ZEPHYR_PATH=../zephyrproject/zephyr
CMAKE_ZEPHYR_COMMAND=west build
CMAKE_ZEPHYR_FLAGS=-b $(TARGET)

all: flash

compile:
	$(CMAKE_ZEPHYR_COMMAND) $(CMAKE_ZEPHYR_FLAGS)

# FIXME: This is entirely board specific, for example the pi pico will vibe differently
flash:
	west flash

clean:
	rm -rf build/
	mkdir build
