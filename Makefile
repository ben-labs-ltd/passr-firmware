.ONESHELL:

TARGET=nrf52840dk_nrf52840

CMAKE_ZEPHYR_PATH=../zephyrproject/zephyr
CMAKE_ZEPHYR_COMMAND=west build
CMAKE_ZEPHYR_FLAGS=-b $(TARGET)

all: compile flash

setup:
	west init
	west update

compile:
	$(CMAKE_ZEPHYR_COMMAND) $(CMAKE_ZEPHYR_FLAGS)

flash: compile
	cp build/zephyr/zephyr.hex ./zephyr.hex
	echo "Copied zephyr.hex to project root"
	nrfjprog -f nrf52 --program zephyr.hex --sectorerase --verify --log --reset
	minicom -D /dev/ttyACM0 -b 115200

listen:
	minicom -D /dev/ttyACM0 -b 115200

clean:
	rm -rf build/
	mkdir build
