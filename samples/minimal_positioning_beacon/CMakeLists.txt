# SPDX-License-Identifier: Apache-2.0

# File modifications
# Copyright (c) 2020 Mohammed Nawabuddin
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.8.2)

set(NRF_SUPPORTED_BOARDS
	nrf5340pdk_nrf5340_cpunet
	nrf52dk_nrf52832
	nrf52840dk_nrf52840
	nrf52840dongle_nrf52840
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(beacon)

FILE(GLOB app_sources
	src/*.c
	$ENV{ZEPHYR_BASE}/../modules/bsim_hw_models/nrf_hw_models/src/HW_models/crc_ble.c
)
target_sources(app PRIVATE
	${app_sources}
)
