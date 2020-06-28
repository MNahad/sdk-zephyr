#include "radio.h"

#include <drivers/clock_control.h>
#include <drivers/clock_control/nrf_clock_control.h>
#include <kernel.h>

#define OFFSET_TO_FREQ(_offset)	(2400 + _offset)
#define FREQ_TO_OFFSET(_freq)	(_freq - 2400)

#define DT_DRV_COMPAT			nordic_nrf_clock

extern void append_crc_ble(u8_t *buffer, u32_t len, u32_t crc_init);

static const nrfx_timer_t timer = NRFX_TIMER_INSTANCE(0);

static const u8_t data[] = {
	0x42, 0x12,
	0x06, 0x05, 0x04, 0x03, 0x02, 0xC1,
	0x0B, 0x09, 0x41, 0x6F, 0x41, 0x20, 0x42, 0x65, 0x61, 0x63, 0x6F, 0x6E
};

static void *tx_packet = NULL;
static size_t tx_size = sizeof(data) + 3;

static struct radio_param_config {
	nrf_radio_mode_t mode;
	u8_t txpower;
	u8_t freq_offset;
	u8_t packets;
	u32_t delay_ms;
	u8_t duty_cycle;
} config = {
	.mode = NRF_RADIO_MODE_BLE_1MBIT,
	.txpower = RADIO_TXPOWER_TXPOWER_0dBm,
	.freq_offset = 2,
	.packets = 1,
	.delay_ms = 10,
	.duty_cycle = 50,
};

static u8_t switch_bit_endianness(u8_t octet) {
  return ((octet * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
}

static void whiten(u8_t *buffer, size_t len, u8_t freq_offset) {
	u8_t reg = 0x40;
	switch (freq_offset) {
		case 2:
			reg |= 0x25;
			break;
		case 26:
			reg |= 0x26;
			break;
		case 80:
			reg |= 0x27;
			break;
		default:
			reg |= (freq_offset-0x02)/0x02;
			break;
	}

	for (int i = 0; i < len; i++) {
		for (int j = 7; j >= 0; j--) {
			buffer[i] = (buffer[i] & ~(0x01 << j)) |
				((buffer[i] ^ (reg << j)) & (0x01 << j));
			reg = ((reg >> 1) & 0x3B) |
				((reg << 6) & 0x40) |
				(((reg << 2) ^ (reg >> 1)) & 0x04);
		}
	}
}

static bool in_progress;

static void radio_config() {
	nrf_radio_packet_conf_t packet_conf;

	nrf_radio_modecnf0_set(NRF_RADIO, true, RADIO_MODECNF0_DTX_Center);
	nrf_radio_crc_configure(
		NRF_RADIO,
		RADIO_CRCCNF_LEN_Disabled,
		NRF_RADIO_CRC_ADDR_INCLUDE,
		0
	);

	nrf_radio_txaddress_set(NRF_RADIO, 0);
	u32_t access_addr = 0x8E89BED6;
	nrf_radio_prefix0_set(NRF_RADIO, access_addr >> (8*3));
	nrf_radio_base0_set(NRF_RADIO, (access_addr & ((1 << 8*3)-1)) << 8);

	memset(&packet_conf, 0, sizeof(packet_conf));
	packet_conf.plen = NRF_RADIO_PREAMBLE_LENGTH_8BIT;
	packet_conf.lflen = 8;
	packet_conf.maxlen = (tx_size - 1);
	packet_conf.statlen = 0;
	packet_conf.balen = 3;
	packet_conf.big_endian = true;
	packet_conf.whiteen = false;

	nrf_radio_packet_configure(NRF_RADIO, &packet_conf);
}

static void generate_payload(u8_t *payload, size_t len) {
	append_crc_ble(payload, len-3, 0x555555);
	for (u8_t i = 0; i < len; i++) {
		payload[i] = switch_bit_endianness(payload[i]);
	}
	whiten(payload, len, config.freq_offset);
}

static void generate_rf_packet() {
	u8_t payload[sizeof(data)+3];
	for (u8_t i = 0; i < sizeof(data); i++) {
		payload[i] = data[i];
	}
	generate_payload(payload, sizeof(payload));

	k_free(tx_packet);
	tx_packet = k_malloc(sizeof(payload) * sizeof(u8_t));
	for (u8_t i = 0; i < sizeof(payload); i++) {
		((u8_t *) tx_packet)[i] = payload[i];
	}

	nrf_radio_packetptr_set(NRF_RADIO, tx_packet);
}

void radio_setup() {
	radio_config();
	generate_rf_packet();
	nrf_radio_shorts_enable(
		NRF_RADIO,
		NRF_RADIO_SHORT_READY_START_MASK | NRF_RADIO_SHORT_END_DISABLE_MASK
	);

	nrf_radio_mode_set(NRF_RADIO, config.mode);
	nrf_radio_txpower_set(NRF_RADIO, config.txpower);
	nrf_radio_frequency_set(NRF_RADIO, OFFSET_TO_FREQ(config.freq_offset));

	nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_END);
	nrf_radio_int_enable(NRF_RADIO, NRF_RADIO_INT_END_MASK);
}

void timer_setup() {
	nrfx_timer_disable(&timer);
	nrf_timer_shorts_disable(timer.p_reg, ~0);
	nrf_timer_int_disable(timer.p_reg, ~0);

	nrfx_timer_extended_compare(
		&timer,
		NRF_TIMER_CC_CHANNEL0,
		nrfx_timer_us_to_ticks(&timer, 100ULL * 1000ULL),
		NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
		true
	);

	nrfx_timer_clear(&timer);
}

void run_timer() {
	nrfx_timer_enable(&timer);
}

static void tx_radio() {
	nrf_radio_shorts_enable(
		NRF_RADIO,
		NRF_RADIO_SHORT_READY_START_MASK | NRF_RADIO_SHORT_END_DISABLE_MASK
	);

	nrf_radio_mode_set(NRF_RADIO, config.mode);
	nrf_radio_txpower_set(NRF_RADIO, config.txpower);
	nrf_radio_frequency_set(NRF_RADIO, OFFSET_TO_FREQ(config.freq_offset));

	nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_END);
	nrf_radio_int_enable(NRF_RADIO, NRF_RADIO_INT_END_MASK);
	nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_TXEN);
	while (!nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_END)) {}
}

void transmit_packets(u8_t *packets) {
	if (in_progress) {
		radio_cancel();
		in_progress = false;
	}
	config.packets = *packets;
	tx_radio();
	in_progress = true;
}

void set_mode(nrf_radio_mode_t *mode) {
	config.mode = *mode;
}

void set_offset(u8_t *freq_offset) {
	config.freq_offset = *freq_offset;
}

void set_txpower(u8_t *tx_power) {
	config.txpower = *tx_power;
}

void set_delay(u32_t *delay_ms) {
	config.delay_ms = *delay_ms;
}

void radio_cancel(void) {
	nrfx_timer_disable(&timer);
	nrf_radio_shorts_set(NRF_RADIO, 0);
	nrf_radio_int_disable(NRF_RADIO, ~0);
	nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);

	nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_DISABLE);
	while (!nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_DISABLED)) {}
	nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_DISABLED);
}

static void clock_init(void) {
	int err;
	struct device *clock;
	enum clock_control_status clock_status;

	clock = device_get_binding(DT_INST_LABEL(0));
	if (!clock) {
		return;
	}

	err = clock_control_on(clock, CLOCK_CONTROL_NRF_SUBSYS_HF);
	if (err) {
		return;
	}

	do {
		clock_status = clock_control_get_status(clock, CLOCK_CONTROL_NRF_SUBSYS_HF);
	} while (clock_status != CLOCK_CONTROL_STATUS_ON);
}

static void timer_handler(nrf_timer_event_t event_type, void *context) {
	if (event_type == NRF_TIMER_EVENT_COMPARE0) {
		nrf_radio_task_trigger(NRF_RADIO, NRF_RADIO_TASK_TXEN);
	}
}

static void timer_init() {
	nrfx_timer_config_t timer_cfg = {
		.frequency = NRF_TIMER_FREQ_1MHz,
		.mode      = NRF_TIMER_MODE_TIMER,
		.bit_width = NRF_TIMER_BIT_WIDTH_24,
		.p_context = (void *) &config,
	};

	nrfx_err_t err = nrfx_timer_init(&timer, &timer_cfg, timer_handler);
	if (err != NRFX_SUCCESS) {
		return;
	}
}

static void radio_handler(void *context) {
	if (nrf_radio_event_check(NRF_RADIO, NRF_RADIO_EVENT_END)) {
		nrf_radio_event_clear(NRF_RADIO, NRF_RADIO_EVENT_END);
	}
}

int radio_init(struct device *dev) {
	ARG_UNUSED(dev);

	clock_init();
	timer_init();
	IRQ_CONNECT(
		TIMER0_IRQn,
		NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY,
		nrfx_timer_0_irq_handler,
		NULL,
		0
	);
	irq_connect_dynamic(RADIO_IRQn, 7, radio_handler, (void *) &config, 0);
	irq_enable(RADIO_IRQn);

	return 0;
}
