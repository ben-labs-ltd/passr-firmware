#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/spi.h>

// SPI Master Interface on SPI1 of the rp2040
const struct device* spi1_dev = DEVICE_DT_GET(DT_NODELABEL(spi1));

// SPI but broken out
#define	EPD_RST_PIN = 12;
#define	EEPD_DC_PIN = 8;
#define	EEPD_BUSY_PIN = 13;
#define	EEPD_CS_PIN = 9;
#define	EEPD_CLK_PIN = 10;
#define	EEPD_MOSI_PIN = 11;

// GPIO - so we can have a status LED
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void main(void)
{
    // set up the led
    while (!device_is_ready(led.port)) { }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    gpio_pin_set_dt(&led, GPIO_OUTPUT_LOW);

    while (!device_is_ready(spi1_dev)) { }

    gpio_pin_set_dt(&led, GPIO_OUTPUT_HIGH);

    while (1) {
        k_msleep(500);
        gpio_pin_toggle_dt(&led);
    }
}

