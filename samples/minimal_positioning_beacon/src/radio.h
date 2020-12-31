/*
 * Based on samples/peripheral/radio_test/src/radio_test.c
 * From https://github.com/nrfconnect/sdk-nrf
 * 
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 * 
 * Depends on function append_crc_ble()
 * Located at modules/bsim_hw_models/nrf_hw_models/
 *
 * Copyright (c) 2017 Oticon A/S
 *
 * SPDX-License-Identifier: Apache-2.0
 */

 /*
  * File modifications
  * Copyright (c) 2020 Mohammed Nawabuddin
  * SPDX-License-Identifier: Apache-2.0
  */

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
