west build -b adafruit_feather_nrf52840 .
nrfutil dfu genpkg --application build/zephyr/zephyr.hex --application-version 0xFF --dev-revision 0xFF --dev-type 0xFFFF --sd-req 0x81 feather_nrf52840_express_bootloader-0.7.0_s140_6.1.1.zip
# nrfutil dfu usb-serial -pkg app.zip -p ttyACM0 // DO NOT FUCKING DO THIS
