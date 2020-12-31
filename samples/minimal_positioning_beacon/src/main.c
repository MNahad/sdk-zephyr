/*
 * Copyright (c) 2020 Mohammed Nawabuddin
 * SPDX-License-Identifier: Apache-2.0
 */

#include "radio.h"

SYS_INIT(radio_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);

void main(void) {
  run_beacon();
}
