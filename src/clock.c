/* clock.c --- 
 * 
 * Filename: clock.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Fri Sep 25 16:26:06 2015
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
#include "test.h"
#include "shell.h" 
#include "chprintf.h"
#include <chstreams.h>
#include "console.h"
#include "stm32f30x_flash.h"
#include "flash_data.h"
#include "clock.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

void cmd_rtcSet(BaseSequentialStream *chp, int argc, char *argv[]) {
  //  int32_t i;
  RTCDateTime time;
  struct tm * ltime;
  time_t epoch;
  
  (void)argv;
  (void)chp;
  (void)argc;
 
  epoch = (time_t) atoi(argv[0]);
  epoch -= 4 * 3600; // Convert from UTC time to EDT time. 
  ltime = localtime(&epoch);
  rtcConvertStructTmToDateTime(ltime, 0, &time);
  rtcSetTime(&RTCD1, &time);
}

void cmd_rtcRead(BaseSequentialStream *chp, int argc, char *argv[]) {
  //  int32_t i;
  RTCDateTime time;
  struct tm ltime;
  (void)argv;
  (void)argc;
  rtcGetTime(&RTCD1, &time);
  rtcConvertDateTimeToStructTm(&time, &ltime, NULL);
  chprintf(chp,"%s\n\r",asctime(&ltime));  
}

/* clock.c ends here */
