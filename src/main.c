/*
  Copyright (c) 2004-2016 The Trustees of Indiana University and Indiana
  University Research and Technology Corporation.

  All rights reserved.

  Additional copyrights may follow.
*/

/*
  Authors: bhimebau

  Primary initializations and console functions.
*/

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
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static virtual_timer_t periodic_time_interval;
static semaphore_t time_to_run_semaphore;

#define UNUSED(x) (void)(x)

static THD_WORKING_AREA(waCounterThread,128);
static THD_FUNCTION(counterThread,arg) {
  UNUSED(arg);
  while (TRUE) {
    led_off();
    chThdSleepMilliseconds(2);
    led_on();
    chThdSleepMilliseconds(4);
  }
}

static THD_WORKING_AREA(waPeriodicTask,128);
static THD_FUNCTION(PeriodicTask,arg) {
  UNUSED(arg);
  // chprintf((BaseSequentialStream*)&SD2,"G");
  while (TRUE) {
    chSemWait(&time_to_run_semaphore);
    // chprintf((BaseSequentialStream*)&SD2,"T");
    PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);
    // power_enter_sleep_mode();
    // chprintf((BaseSequentialStream*)&SD2,"S");
  }
}

static THD_WORKING_AREA(waShell,2048);

static thread_t *shelltp1;
static thread_t *countertp1;


static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;

  for (i = 0; i < argc; i++) {
    chprintf(chp, "%s\n\r", argv[i]);
  }
  //chprintf(chp, "%d\n\r", global_track);
}

static void cmd_stop(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  chprintf(chp, "stopping...\n");
  chThdSleepMilliseconds(200);

  PWR_EnterSTOPMode(((uint32_t)0x00000001), ((uint8_t)0x01));
}

static void cmd_r_data(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argc;
  (void) argv;

  adxl362_read_register(0x0B);
  chprintf(chp, "alarm_called = %d\n", alarm_called);
  //RESET_ALARM;
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
  {"xyz", cmd_xyz},
  {"rdata", cmd_r_data},
  {"allreg",cmd_reg},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD2,
  commands
};

void termination_handler(eventid_t id) {
  (void)id;

  chprintf((BaseSequentialStream*)&SD2, "Shell Died\n\r");

  if (shelltp1 && chThdTerminatedX(shelltp1)) {
    chThdWait(shelltp1);
    chprintf((BaseSequentialStream*)&SD2,
      "Restarting from termination handler\n\r");
    chThdSleepMilliseconds(100);
    shelltp1 = shellCreate(&shell_cfg1, sizeof(waShell), NORMALPRIO);
  }
}

static void periodic_interval_func(void *arg) {
  chSysLockFromISR();
  // chVTSetI(&periodic_time_interval,MS2ST(2000),periodic_interval_func,NULL);
  chSemSignalI(&time_to_run_semaphore);
  chSysUnlockFromISR();
}

/* static evhandler_t fhandlers[] = { */
/*   termination_handler */
/* }; */

/*
 * Application entry point.
 */
int main(void) {
  event_listener_t tel;
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

  // Initialize serial for the console.
  console_serial_init();

  // Print hourly data from flash.
  printHourlyData();

  // Get/convert/print current set time.
  rtcGetTime(&RTCD1, &time);
  rtcConvertDateTimeToStructTm(&time,&ltime, NULL);
  chprintf((BaseSequentialStream*)&SD2,"%s\n\r",asctime(&ltime));

  // Initialize and enable external interrupts via rtc.
  trailRtcInitAlarmSystem();
  RESET_ALARM;
  people_count = 0;

  // Init accelerometer.
  adxl362_init();

  led_init();
  button_init();

  // Turn off board led which is used for debugging.
  led_off();

  chSemObjectInit(&time_to_run_semaphore,0);
  //  chThdSleepMilliseconds(1000);

  // Initialize the periodic timer
  chVTObjectInit(&periodic_time_interval);
  chVTSet(&periodic_time_interval, MS2ST(2000), periodic_interval_func, NULL);

  //chThdSleepMilliseconds(5000);


  /* Initialize the command shell */
  // shellInit();
  /*
   *  setup to listen for the shell_terminated event. This setup will be stored in the tel  * event listner structure in item 0
   */
  // chEvtRegister(&shell_terminated, &tel, 0);

  shelltp1 = shellCreate(&shell_cfg1, sizeof(waShell), NORMALPRIO);
  //chThdRelease(shelltp1);
  // chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);
  chThdCreateStatic(waPeriodicTask, sizeof(waPeriodicTask), NORMALPRIO+1,
    PeriodicTask, NULL);

  //chThdRelease(countertp1);

  /* int alarm_time = (60*5*1000);  //currently 5 minutes */
  /* uint32_t hourly_wakeup = (time.millisecond) + alarm_time; */
  /* char tomorrow = ((time.dayofweek) % 7) + 1; */

  // trailRtcSetAlarm(&RTCD1, 30, &time);

  while(TRUE) {
    //     chEvtDispatch(fhandlers, chEvtWaitOne(ALL_EVENTS));
    chThdSleepMilliseconds(4000);
    // PWR_EnterSTOPMode( ((uint32_t)0x00000001), PWR_STOPEntry_WFI);
  }

/*   while (TRUE){ */
/*     if (button_read()) led_on(); */
/*     else led_off(); */


/*     /\*Shell Dispatcher *\/ */
/*     chEvtDispatch(fhandlers, chEvtWaitOne(ALL_EVENTS)); */

/*     //TODO: */
/*     // rtcSetTime so we are writing proper times */
/*     // Consider making write to flash function that converts time to uint32 */
/*     //   epochdata saves day and year */
/*     //   hourlydata saves hours */
/*     //Test long term time, make sure it's writing proper number of times and that */
/*     //time isn't getting off balance with real time */


/*     //woke up from alarm */
/*     if (alarm_called) { */
/*       trailRtcSetAlarm(&RTCD1, 30, &time); */
/*       if (time.millisecond > hourly_wakeup) { */
/* 	//make a function for this */
/* 	writeHourlyData(getFirstFreeHourly(), *(&people_count)); */
/* 	people_count = 0; */
/* 	hourly_wakeup += alarm_time; */
/*       } */
/*       if (time.dayofweek == tomorrow) { */
/* 	writeHourlyData(getFirstFreeHourly(), *(&people_count)); */
/* 	writeEpochDataWord(getFirstFreeEpoch(), time.day); */
/* 	people_count = 0; */
/* 	hourly_wakeup = (time.millisecond) + alarm_time; */
/* 	tomorrow = ((time.dayofweek) % 7) + 1; */
/*       }  */
/*       RESET_ALARM; */
/*     } */



/*     // rtcConvertDateTimeToStructTm(&time,&ltime, NULL); */
/*     // chprintf((BaseSequentialStream*)&SD2,"Current time:%s\n\r",asctime(&ltime)); */

/*     //PWR_EnterSTOPMode( ((uint32_t)0x00000001), PWR_STOPEntry_WFI); */
/*   } */
/*   //return 0; */
  return (0);

}
