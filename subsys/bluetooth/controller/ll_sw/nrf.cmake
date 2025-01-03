# SPDX-License-Identifier: Apache-2.0

# File modifications
# Copyright (c) 2020 Mohammed Nawabuddin
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_BT_LL_SW_SPLIT)
  zephyr_library_sources(
    ll_sw/nordic/lll/lll.c
    ll_sw/nordic/lll/lll_clock.c
    )
  if(CONFIG_BT_BROADCASTER)
    zephyr_library_sources(
      ll_sw/nordic/lll/lll_adv.c
      )
    zephyr_library_sources_ifdef(
      CONFIG_BT_CTLR_ADV_EXT
      ll_sw/nordic/lll/lll_adv_aux.c
      )
    zephyr_library_sources_ifdef(
      CONFIG_BT_CTLR_ADV_PERIODIC
      ll_sw/nordic/lll/lll_adv_sync.c
      )
    zephyr_library_sources_ifdef(
      CONFIG_BT_CTLR_DF_SOFTCTE
    	$ENV{ZEPHYR_BASE}/../modules/bsim_hw_models/nrf_hw_models/src/HW_models/crc_ble.c
      )
  endif()
  if(CONFIG_BT_OBSERVER)
    zephyr_library_sources(
      ll_sw/nordic/lll/lll_scan.c
      )
    zephyr_library_sources_ifdef(
      CONFIG_BT_CTLR_ADV_EXT
      ll_sw/nordic/lll/lll_scan_aux.c
      )
  endif()
  if(CONFIG_BT_CONN)
    zephyr_library_sources(
      ll_sw/nordic/lll/lll_conn.c
      )
    zephyr_library_sources_ifdef(
      CONFIG_BT_PERIPHERAL
      ll_sw/nordic/lll/lll_slave.c
      )
    zephyr_library_sources_ifdef(
      CONFIG_BT_CENTRAL
      ll_sw/nordic/lll/lll_master.c
      )
  endif()
  zephyr_library_sources_ifdef(
    CONFIG_BT_CTLR_DTM
    ll_sw/nordic/lll/lll_test.c
    )
  zephyr_library_sources_ifdef(
    CONFIG_BT_CTLR_PROFILE_ISR
    ll_sw/nordic/lll/lll_prof.c
    )
  zephyr_library_include_directories(
    ll_sw/nordic/lll
    )
endif()

zephyr_library_sources(
  ll_sw/nordic/hal/nrf5/cntr.c
  ll_sw/nordic/hal/nrf5/ecb.c
  ll_sw/nordic/hal/nrf5/radio/radio.c
  ll_sw/nordic/hal/nrf5/mayfly.c
  ll_sw/nordic/hal/nrf5/ticker.c
  )

zephyr_library_sources_ifdef(
  CONFIG_SOC_FAMILY_NRF
  hci/nordic/hci_vendor.c
  )

zephyr_library_include_directories(
  ll_sw/nordic
  hci/nordic
)
