#include <drivers/gpio.h>

#include "radio.h"

#define LED0_NODE	DT_ALIAS(led0)
#define LED0		DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN0		DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS0		DT_GPIO_FLAGS(LED0_NODE, gpios)

#define LED1_NODE	DT_ALIAS(led1)
#define LED1		DT_GPIO_LABEL(LED1_NODE, gpios)
#define PIN1		DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGS1		DT_GPIO_FLAGS(LED1_NODE, gpios)

#define LED2_NODE	DT_ALIAS(led2)
#define LED2		DT_GPIO_LABEL(LED2_NODE, gpios)
#define PIN2		DT_GPIO_PIN(LED2_NODE, gpios)
#define FLAGS2		DT_GPIO_FLAGS(LED2_NODE, gpios)

#define LED3_NODE	DT_ALIAS(led3)
#define LED3		DT_GPIO_LABEL(LED3_NODE, gpios)
#define PIN3		DT_GPIO_PIN(LED3_NODE, gpios)
#define FLAGS3		DT_GPIO_FLAGS(LED3_NODE, gpios)

static struct device *led0;
static struct device *led1;
static struct device *led2;
static struct device *led3;

SYS_INIT(radio_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);

void main(void) {
	led0 = device_get_binding(LED0);
	gpio_pin_configure(led0, PIN0, GPIO_OUTPUT_ACTIVE | FLAGS0);
	gpio_pin_set(led0, PIN0, 1);

	led1 = device_get_binding(LED1);
	gpio_pin_configure(led1, PIN1, GPIO_OUTPUT_ACTIVE | FLAGS1);
	gpio_pin_set(led1, PIN1, 0);
	led2 = device_get_binding(LED2);
	gpio_pin_configure(led2, PIN2, GPIO_OUTPUT_ACTIVE | FLAGS2);
	gpio_pin_set(led2, PIN2, 0);
	led3 = device_get_binding(LED3);
	gpio_pin_configure(led3, PIN3, GPIO_OUTPUT_ACTIVE | FLAGS3);
	gpio_pin_set(led3, PIN3, 0);

	radio_setup();
	timer_setup();
	run_timer();

	bool isLedOn = true;
 	while (true) {
		gpio_pin_set(led1, PIN1, (u8_t) isLedOn);
		gpio_pin_set(led2, PIN2, (u8_t) isLedOn);
		gpio_pin_set(led3, PIN3, (u8_t) isLedOn);
		isLedOn = !isLedOn;
		k_sleep(K_MSEC(100));
	}
}
