/*
  Copyright (c) 2004-2016 The Trustees of Indiana University and Indiana
  University Research and Technology Corporation.

  All rights reserved.

  Additional copyrights may follow.
*/

/*
  Authors: bhimebau

  Driver file for user button.
*/

#include "ch.h"
#include "hal.h"
#include "button.h"

void button_init(void) {
  palSetPadMode(GPIOC, 7, PAL_MODE_INPUT_PULLUP);
  palSetPad(GPIOC, 9);
}

uint8_t button_read(void) {
  return(palReadPad(GPIOC,7));
}
