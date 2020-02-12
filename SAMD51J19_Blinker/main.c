#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

    bool toggle = true;
    gpio_set_pin_level(PIN_LED, toggle);

	while (1) {
        delay_ms(200);
        toggle = !toggle;
        gpio_set_pin_level(PIN_LED, toggle);
	}
}
