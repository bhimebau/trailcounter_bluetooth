/*
  Copyright (c) 2004-2016 The Trustees of Indiana University and Indiana
  University Research and Technology Corporation.

  All rights reserved.

  Additional copyrights may follow.
*/

/*
  Authors: bhimebau

  Driver for on device led, mainly used to debug timing and as a simplistic
  status check.
*/

#include "ch.h"
#include "hal.h"
#include "led.h"

void led_init(void) {
  palSetPadMode(GPIOC, 9, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPad(GPIOC, 9);
}

void led_on(void) {
  palSetPad(GPIOC, 9);
}

void led_off(void) {
  palClearPad(GPIOC, 9);
}
