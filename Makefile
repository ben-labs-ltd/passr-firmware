.ONESHELL:

TARGET=rpi_pico

CMAKE_ZEPHYR_PATH=../zephyrproject/zephyr
CMAKE_ZEPHYR_COMMAND=west build
CMAKE_ZEPHYR_FLAGS=-b $(TARGET)

all: compile flash_pico

setup:
	west init
	west update

compile:
	$(CMAKE_ZEPHYR_COMMAND) $(CMAKE_ZEPHYR_FLAGS)
	cp build/zephyr/zephyr.uf2 ./zephyr.uf2
	echo "Copied zephyr.uf2 to project root"

flash_pico:
	# mount then copy the uf2
	sudo mount -L RPI-RP2 /mnt
	sudo cp zephyr.uf2 /mnt
	echo "Copied zephyr.uf2 to pico"

# FIXME: This is entirely board specific, for example the pi pico will vibe differently
flash:
	west flash

clean:
	rm -rf build/
	mkdir build
