/* button.c ---
 *
 * Filename: button.c
 * Description:
 * Author: Bryce Himebaugh
 * Maintainer:
 * Created: Thu Feb  4 14:55:05 2016
 * Last-Updated:
 *           By:
 *     Update #: 0
 * Keywords:
 * Compatibility:
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change log:
 *
 *
 */

/* Copyright (c) 2004-2007 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

/* Code: */
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

/* button.c ends here */
