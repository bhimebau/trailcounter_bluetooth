/*
    ChibiOS - Copyright (C) 2006-2014 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "chprintf.h"
#include <chstreams.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// https://github.com/fpoussin/ChibiOS-Examples/blob/master/STM32F4-DAC/main.c

#define UNUSED(x) (void)(x)

static const DACConfig daccfg1 = {
  DAC_MODE_CONTINUOUS,
  960*DAC_BUFFER_SIZE, /* Multiply the buffer size to the desired frequency in Hz */
  dac_buffer, /* Pointer to the first buffer */
  NULL, /* Pointer to the second buffer */
  DAC_BUFFER_SIZE, /* Buffers size */
  daccb, /* End of transfer callback */
  dacerrcb, /* Error callback */
  /* STM32 specific config starts here */
  DAC_DHRM_12BIT_RIGHT, /* data holding register mode */
  0 /* CR flags */
};

/* Thread that blinks North LED */
static THD_WORKING_AREA(waCounterThread,128);
static THD_FUNCTION(counterThread,arg) {
  UNUSED(arg);
  while (TRUE) {
    palSetPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(500);
    palClearPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

/* Thread that write the status of the user button to the south LED */
static THD_WORKING_AREA(waButtonThread,128);
static THD_FUNCTION(buttonThread,arg) {
  UNUSED(arg);
  while (TRUE) {
    if (palReadPad(GPIOA, GPIOA_BUTTON)) { 
      palSetPad(GPIOE, GPIOE_LED10_RED); 
    }
    else {
      palClearPad(GPIOE, GPIOE_LED10_RED); 
    }
    chThdSleepMilliseconds(10);
  }
  return 0;
}

/* Thread that echos characters received from the console */
static THD_WORKING_AREA(waEchoThread,128);
static THD_FUNCTION(echoThread,arg) {
  UNUSED(arg);
  uint8_t ch;
  while (TRUE) {
    chnRead((BaseSequentialStream*)&SD1,&ch,1);
    chnWrite((BaseSequentialStream*)&SD1,&ch,1);
  }
  return 0;
}

/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 1 using the driver default configuration.
   * PC4(RX) and PC5(TX). The default baud rate is 9600.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));
  chprintf((BaseSequentialStream*)&SD1, "\n\rUp and Running\n\r");


  /*  
   *
   *
   */
  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));



  /*
   * Creates the threads.
   */
  chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);
  chThdCreateStatic(waButtonThread, sizeof(waButtonThread), NORMALPRIO+1, buttonThread, NULL);
  chThdCreateStatic(waEchoThread, sizeof(waEchoThread), NORMALPRIO+1, echoThread, NULL);

  /*
    Main spins here while the threads do all of the work. 
  */ 
  while (TRUE);
}
