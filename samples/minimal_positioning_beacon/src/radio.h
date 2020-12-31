#ifndef RADIO_H
#define RADIO_H

#include <string.h>
#include <zephyr.h>
#include <zephyr/types.h>
#include <nrfx/hal/nrf_radio.h>
#include <device.h>

int radio_init(const struct device *dev);
void set_mode(nrf_radio_mode_t *mode);
void set_offset(uint8_t *freq_offset);
void set_txpower(uint8_t *tx_power);
void set_delay(uint32_t *delay_ms);

void run_beacon(void);
void stop_beacon(void);
#endif
