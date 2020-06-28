#ifndef RADIO_H
#define RADIO_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <init.h>
#include <zephyr.h>
#include <zephyr/types.h>
#include <sys/util.h>
#include <nrfx/hal/nrf_radio.h>
#include <nrfx_timer.h>
#include <device.h>

void transmit_packets(u8_t *packets);
void set_mode(nrf_radio_mode_t *mode);
void set_offset(u8_t *freq_offset);
void set_txpower(u8_t *tx_power);
void set_delay(u32_t *delay_ms);
void radio_cancel(void);
int radio_init(struct device *dev);

void radio_setup();
void timer_setup();
void run_timer();
#endif
