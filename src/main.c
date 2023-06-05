#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/printk.h>

#define SLEEP_TIME_MS   1000

#define LED0_NODE DT_ALIAS(led0)
#define SPI_MASTER DT_NODELABEL(spi_master)
#define SPI_SLAVE  DT_NODELABEL(spi_slave)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// SPI master functionality
const struct device *spi_dev;
static struct k_poll_signal spi_done_sig = K_POLL_SIGNAL_INITIALIZER(spi_done_sig);

const struct spi_cs_control spim_cs = {
	.gpio = SPI_CS_GPIOS_DT_SPEC_GET(DT_NODELABEL(reg_spi_master)),
	.delay = 0,
};

static const struct spi_config spi_cfg = {
    .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB |
        SPI_MODE_CPOL | SPI_MODE_CPHA,
    .frequency = 10000,
    .slave = 0,
    .cs = {{(const struct device*)&spim_cs}},
};

static void spi_init(void)
{
    printk("SPI master init\n");
	spi_dev = DEVICE_DT_GET(SPI_MASTER);
	if(!device_is_ready(spi_dev)) {
		printk("SPI master device not ready!\n");
	}
	if(!device_is_ready(spim_cs.gpio.port)){
		printk("SPI master chip select device not ready!\n");
	}
}


static int spi_write_test_msg(void)
{
	static uint8_t counter = 0;
	static uint8_t tx_buffer[2];
	static uint8_t rx_buffer[2];

	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer)
	};
	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1
	};

    // Update the TX buffer with a rolling counter
	tx_buffer[0] = counter++;
	printk("SPI TX: 0x%.2x, 0x%.2x\n", tx_buffer[0], tx_buffer[1]);

	// Reset signal
	k_poll_signal_reset(&spi_done_sig);

	// Start transaction
	int error = spi_transceive(spi_dev, &spi_cfg, &tx, NULL);
	if(error != 0){
		printk("SPI transceive error: %i\n", error);
		return error;
	}

	// Wait for the done signal to be raised and log the rx buffer
	int spi_signaled, spi_result;
	do{
		k_poll_signal_check(&spi_done_sig, &spi_signaled, &spi_result);
	} while(spi_signaled == 0);
	printk("SPI RX: 0x%.2x, 0x%.2x\n", rx_buffer[0], rx_buffer[1]);
	return 0;
}

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

	printk("SPI master started\n");

	while (1) {
		spi_write_test_msg();
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
            printk("Error toggling LED pin\n");
			return;
		}
		k_msleep(SLEEP_TIME_MS);
	}
}
