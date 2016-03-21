/*
  Copyright (c) 2004-2016 The Trustees of Indiana University and Indiana
  University Research and Technology Corporation.

  All rights reserved.

  Additional copyrights may follow.
*/

/*
  Authors: bhimebau

  Console driver and console serial setup.
*/

#include "ch.h"
#include "hal.h"
#include "console.h"

/*
 * Activates the serial driver 1 using the driver default configuration.
 * PC4(RX) and PC5(TX). The default baud rate is 9600.
 */
void console_serial_init(void) {
  sdStart(&SD2, NULL);
  //  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  //  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));
}
