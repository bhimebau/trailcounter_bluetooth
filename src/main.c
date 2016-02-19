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
#include "console.h"
#include "stm32f30x_flash.h"
#include "stm32f30x_pwr.h"
#include "flash_data.h"
#include "clock.h"
#include "adxl362.h"
#include "led.h"
#include "button.h"
#include "power_mode.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static virtual_timer_t periodic_tim_int;
static semaphore_t time2run; 

#define UNUSED(x) (void)(x)

static THD_WORKING_AREA(waPeriodicTask,128);
static THD_FUNCTION(PeriodicTask,arg) {
  UNUSED(arg);
  while (TRUE) {
    chSemWait(&time2run);
    //    PWR_EnterSleepMode(PWR_SLEEPEntry_WFI); 
  }
}

static THD_WORKING_AREA(waShell,2048);

static thread_t *shelltp1;
static thread_t *PeriodicTaskp1;

static const ShellCommand commands[] = {
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD2,
  commands
};

static void periodic_int_func(void *arg) {
  (void) arg;

  chSysLockFromISR();
  chVTSetI(&periodic_tim_int,MS2ST(250),periodic_int_func,NULL);
  chSemSignalI(&time2run);
  chSysUnlockFromISR();
}

int main(void) {
  halInit();
  chSysInit();
  extStart(&EXTD1, &trailExtcfg);
  // ClockInited = true;
  console_init();
  led_init();
  button_init();
  extStart(&EXTD1, &trailExtcfg);
  led_off();

  chSemObjectInit(&time2run,0);
  chVTObjectInit(&periodic_tim_int); // Initialize the periodic timer
  chVTSet(&periodic_tim_int, MS2ST(250), periodic_int_func, NULL);
  
  /* Initialize the command shell */ 
  shellInit();
  shelltp1 = shellCreate(&shell_cfg1, sizeof(waShell), NORMALPRIO);
  PeriodicTaskp1 = chThdCreateStatic(waPeriodicTask, sizeof(waPeriodicTask), NORMALPRIO+1, PeriodicTask, NULL);
  while(TRUE) {
    chThdSleepMilliseconds(4000);
  }
  return (0);

}


/* main.c ends here */
