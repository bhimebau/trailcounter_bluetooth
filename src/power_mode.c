/*
  Copyright (c) 2004-2016 The Trustees of Indiana University and Indiana
  University Research and Technology Corporation.

  All rights reserved.

  Additional copyrights may follow.
*/

/*
  Authors: bhimebau

  Driver and functions for managing system power mode.  Currently only used to
  enter sleep.
*/

#include "ch.h"
#include "hal.h"

void power_enter_sleep_mode(void) {
  SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
  __WFI();
}
