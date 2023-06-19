#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   100

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define SPI_MASTER DT_NODELABEL(spi_master)

// SPI 
const struct device *spi_dev;
static struct k_poll_signal spi_done_sig = K_POLL_SIGNAL_INITIALIZER(spi_done_sig);

struct spi_cs_control spim_cs = {
	.gpio = SPI_CS_GPIOS_DT_SPEC_GET(DT_NODELABEL(reg_spi_master)),
	.delay = 0,
};

static void spi_init(void)
{
	spi_dev = DEVICE_DT_GET(SPI_MASTER);
	if(!device_is_ready(spi_dev)) {
		printk("SPI master device not ready!\n");
	}
	if(!device_is_ready(spim_cs.gpio.port)){
		printk("SPI master chip select device not ready!\n");
	}
}

static const struct spi_config spi_cfg = {
	.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
	.frequency = 4000000,
	.slave = 0,
	.cs = &spim_cs,
};

static int spi_write_8(uint8_t command) {
    const struct spi_buf tx_buf = {
        .buf = &command,
        .len = sizeof(command)
    };

    const struct spi_buf_set tx = {
        .buffers = &tx_buf,
        .count = 1
    };

    k_poll_signal_reset(&spi_done_sig);
    int error = spi_write(spi_dev, &spi_cfg, &tx);
    if(error != 0){
        printk("SPI write error: %i\n", error);
        return error;
    }
}

static int spi_write_buf(uint8_t* buf, int len) {
    const struct spi_buf tx_buf = {
        .buf = buf,
        .len = len
    };

    const struct spi_buf_set tx = {
        .buffers = &tx_buf,
        .count = 1
    };

    k_poll_signal_reset(&spi_done_sig);
    int error = spi_write(spi_dev, &spi_cfg, &tx);
    if(error != 0){
        printk("SPI write error: %i\n", error);
        return error;
    }
}

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void main(void)
{
	int ret;

	if (!device_is_ready(led.port)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	spi_init();

    k_msleep(SLEEP_TIME_MS);

    spi_write_8(0x12);

    k_msleep(SLEEP_TIME_MS);

    spi_write_8(0x46);
    spi_write_8(0xF7);

    k_msleep(SLEEP_TIME_MS);

    spi_write_8(0x47);
    spi_write_8(0xF7);

    k_msleep(SLEEP_TIME_MS);

	while (1) {
	    gpio_pin_toggle_dt(&led);
		k_msleep(SLEEP_TIME_MS);
    }
}
