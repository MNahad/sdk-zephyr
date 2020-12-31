#include "radio.h"

SYS_INIT(radio_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);

void main(void) {
  run_beacon();
}
