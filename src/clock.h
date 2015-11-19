/* clock.h --- 
 * 
 * Filename: clock.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Fri Sep 25 16:36:16 2015
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
#include "chprintf.h"
#include <chstreams.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

extern volatile int alarm_called;
extern volatile uint16_t people_count;

extern EXTConfig trailExtcfg;

#define RESET_ALARM alarm_called = 0;
#define SET_ALARM alarm_called = 1;

void trailRtcInitAlarmSystem(void);
void trailRtcSetAlarm(RTCDriver *rtcp, uint8_t offset,  RTCDateTime *time);
void cmd_rtcSet(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_rtcRead(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_enableWakeup(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_sleep(BaseSequentialStream *chp, int argc, char *argv[]);


/* clock.h ends here */
