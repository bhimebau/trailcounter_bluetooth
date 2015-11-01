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
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define UNUSED(x) (void)(x)
static THD_WORKING_AREA(waShell,2048);

static thread_t *shelltp1;

static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;

  (void)argv;

  for (i=0;i<argc;i++) {
    chprintf(chp, "%s\n\r", argv[i]);
  }
  //chprintf(chp, "%d\n\r", global_track);
}

static void cmd_stop(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)chp; (void)argc; (void)argv; 

  chprintf(chp, "Going sleep...\r\n");
  chThdSleepMilliseconds(200);

  PWR_EnterSTOPMode(((uint32_t)0x00000001), ((uint8_t)0x01));
}

static const ShellCommand commands[] = {
  {"myecho", cmd_myecho},
  {"rtcset", cmd_rtcSet},
  {"rtcread", cmd_rtcRead},
  {"enablewake", cmd_enableWakeup},
  {"sleep", cmd_sleep},
  {"stop", cmd_stop},
  {"accelwrite", cmd_adxl362_write},
  {"accelread", cmd_adxl362_read},
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
  //event_listener_t tel;
  RTCDateTime time;
  struct tm ltime;

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
  console_init();

  //print flash, init accel
  printEpochData();
  adxl362_init();
  
  //get/convert/print current set time
  rtcGetTime(&RTCD1, &time);
  rtcConvertDateTimeToStructTm(&time,&ltime, NULL);
  chprintf((BaseSequentialStream*)&SD2,"%s\n\r",asctime(&ltime));
  
  //initialize and enable external interrupts
  trailRtcInitAlarmSystem();
  RESET_ALARM;

  while (TRUE){
    /*
    if (alarm_called == 1) {
      chprintf((BaseSequentialStream*)&SD2,"Woken by accelerometer\n\r");
    }
    
    else if (alarm_called == 2) {
      chprintf((BaseSequentialStream*)&SD2,"%Woken by alarm\n\r");
    }
    RESET_ALARM;
    */
    //TODO:  instead of printing, write to flash
    //Also update current time so we are writing proper times
    //Consider making write to flash function that converts time
    // to structure for flash arrays, and it writes to both
    // hourly and daily arrays based on time
    
    trailRtcSetAlarm(&RTCD1, 20, &time);
    writeEpochDataWord(getFirstFreeEpoch(), (time.millisecond/1000)%60);

    //rtcConvertDateTimeToStructTm(&time,&ltime, NULL);
    //chprintf((BaseSequentialStream*)&SD2,"Current time:%s\n\r",asctime(&ltime));

    chThdSleepMilliseconds(500);
    PWR_EnterSTOPMode( ((uint32_t)0x00000001), PWR_STOPEntry_WFI);
  }
  return 0;


}


/* main.c ends here */
