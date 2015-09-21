/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Sun Sep 20 09:32:19 2015
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

/* Copyright (c) 2014-2015 Analog Computing Solutions  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h" 
#include "chprintf.h"
#include <chstreams.h>
#include "gyro.h"
#include "caann.h"
#include "console.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define UNUSED(x) (void)(x)
static THD_WORKING_AREA(waShell,2048);

static thread_t *shelltp1;

/* Thread that blinks North LED as an "alive" indicator */
static THD_WORKING_AREA(waCounterThread,128);
static THD_FUNCTION(counterThread,arg) {
  UNUSED(arg);
  while (TRUE) {
    palSetPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(500);
    palClearPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(500);
  }
}

static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;

  (void)argv;

  for (i=0;i<argc;i++) {
    chprintf(chp, "%s\n\r", argv[i]);
  }
}

static const ShellCommand commands[] = {
  {"myecho", cmd_myecho},
  {"gr",cmd_gyro_read},
  {"cr",cmd_gyro_read},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD2,
  commands
};

static void termination_handler(eventid_t id) {

  (void)id;
  chprintf((BaseSequentialStream*)&SD2, "Shell Died\n\r");

  if (shelltp1 && chThdTerminatedX(shelltp1)) {
    chThdWait(shelltp1);
    chprintf((BaseSequentialStream*)&SD2, "Restarting from termination handler\n\r");
    chThdSleepMilliseconds(100);
    shelltp1 = shellCreate(&shell_cfg1, sizeof(waShell), NORMALPRIO);
  }
}

static evhandler_t fhandlers[] = {
  termination_handler
};

/*
 * Application entry point.
 */

int main(void) {
  event_listener_t tel;
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  // Board Specific Initilizations
  gyro_init();
  caann_init();
  console_init();
  
  //  chprintf((BaseSequentialStream*)&SD2, "\n\rUp and Running\n\r");
  // chprintf((BaseSequentialStream*)&SD2, "Gyro Whoami Byte = 0x%02x\n\r",gyro_read_register(0x0F));

  /* Initialize the command shell */ 
  shellInit();
  /* 
   *  setup to listen for the shell_terminated event. This setup will be stored in the tel  * event listner structure in item 0
  */
  chEvtRegister(&shell_terminated, &tel, 0);

  shelltp1 = shellCreate(&shell_cfg1, sizeof(waShell), NORMALPRIO);
  chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);

  while (TRUE) {
    chEvtDispatch(fhandlers, chEvtWaitOne(ALL_EVENTS));
  }
 }


/* main.c ends here */
