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

#define BLINK_T1 10000
#define BLINK_T2 125

/* Relevant URLS
 * Docs: http://chibios.sourceforge.net/html/index.html
 * 
 * Threads
 * http://www.chibios.org/dokuwiki/doku.php?id=chibios:howtos:createthread
 * 
 * Command Shell: 
 * http://forum.chibios.org/phpbb/viewtopic.php?f=4&t=2056
 * https://github.com/steve-bate/ChibiOS-RPi/blob/master/demos/Posix-GCC/main.c  
 * 
 * Scheduler
 * Take a look at the environment variables in chconf.h. Turn on classic tick mode
 * 
 * Bootloader
 * https://my.st.com/public/STe2ecommunities/mcu/Lists/cortex_mx_stm32/Flat.aspx?RootFolder=%2Fpublic%2FSTe2ecommunities%2Fmcu%2FLists%2Fcortex_mx_stm32%2FCustom%20Bootloader&FolderCTID=0x01200200770978C69A1141439FE559EB459D7580009C4E14902C3CDE46A77F0FFD06506F5B&currentviews=4009
 * http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00047998.pdf
 * 

*/

/*
 * Blinker thread #1.
 */

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;

  chRegSetThreadName("blinker");
  while (true) {
    /* palSetPad(GPIOE, GPIOE_LED3_RED); */
    /* chThdSleepMilliseconds(BLINK_T1); */
    /* palClearPad(GPIOE, GPIOE_LED3_RED); */
    /* chThdSleepMilliseconds(BLINK_T1); */
    /* palSetPad(GPIOE, GPIOE_LED7_GREEN); */
    /* chThdSleepMilliseconds(BLINK_T1); */
    /* palClearPad(GPIOE, GPIOE_LED7_GREEN); */
    /* chThdSleepMilliseconds(BLINK_T1); */
    /* palSetPad(GPIOE, GPIOE_LED10_RED); */
    /* chThdSleepMilliseconds(BLINK_T1); */
    /* palClearPad(GPIOE, GPIOE_LED10_RED); */
    /* chThdSleepMilliseconds(BLINK_T1); */
    /* palSetPad(GPIOE, GPIOE_LED6_GREEN); */
    /* chThdSleepMilliseconds(BLINK_T1); */
       palClearPad(GPIOE, GPIOE_LED6_GREEN); 
    /* chThdSleepMilliseconds(BLINK_T1); */
    //    chThdSleepMilliseconds(1);
  }
}

/*
 * Blinker thread #2.
 */
static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Thread2, arg) {

  (void)arg;

  chRegSetThreadName("blinker");
  while (true) {
    chThdSleepMilliseconds(BLINK_T2);
    palSetPad(GPIOE, GPIOE_LED5_ORANGE);
    chThdSleepMilliseconds(BLINK_T2);
    palClearPad(GPIOE, GPIOE_LED5_ORANGE);
    chThdSleepMilliseconds(BLINK_T2);
    palSetPad(GPIOE, GPIOE_LED9_BLUE);
    chThdSleepMilliseconds(BLINK_T2);
    palClearPad(GPIOE, GPIOE_LED9_BLUE);
    chThdSleepMilliseconds(BLINK_T2);
    palSetPad(GPIOE, GPIOE_LED8_ORANGE);
    chThdSleepMilliseconds(BLINK_T2);
    palClearPad(GPIOE, GPIOE_LED8_ORANGE);
    chThdSleepMilliseconds(BLINK_T2);
    palSetPad(GPIOE, GPIOE_LED4_BLUE);
    chThdSleepMilliseconds(BLINK_T2);
    palClearPad(GPIOE, GPIOE_LED4_BLUE);
  }
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
   * PA9(TX) and PA10(RX) are routed to USART1.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));
  chprintf((BaseSequentialStream*)&SD1, "\n\rUp and Running\n\r");

  /*
   * Creates the example threads.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), HIGHPRIO, Thread2, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched.
   */
  while (TRUE) {
    if (palReadPad(GPIOA, GPIOA_BUTTON))
      TestThread(&SD1);
    chThdSleepMilliseconds(500);
  }
}
