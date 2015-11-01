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


/* Implementation of External Interrupts */

volatile int alarm_called;

static void extcb(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;

  chSysLockFromISR();
  alarm_called = 1;

  chSysUnlockFromISR();
}

static void extcb1(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;

  chSysLockFromISR();

  //if it wakes from stop then it needs to re-init the clock for print
  //i'm not sure if this is required when we go ultra low power
  //needs to be tested
  stm32_clock_init();
  alarm_called = 2;

  chSysUnlockFromISR();
}

EXTConfig trailExtcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    // Enable interrupt on PA4 from the accelerometer
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extcb},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    // Enable interrupt from the RTC alarm
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA , extcb1},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

/* End Interrupt */

static uint8_t RTC_ByteToBcd2(uint8_t Value)
{
  uint8_t bcdhigh = 0;
  
  while (Value >= 10)
  {
    bcdhigh++;
    Value -= 10;
  }

  return  ((uint8_t)(bcdhigh << 4) | Value);
}

void trailRtcInitAlarmSystem(void)
{
  extStart(&EXTD1, &trailExtcfg);
  extChannelEnable(&EXTD1, EXT_MODE_GPIOA);
}

void trailRtcSetAlarm(RTCDriver *rtcp, uint8_t offset, RTCDateTime *time)
{
  uint8_t i;
  RTCAlarm alarmspec;

  //reset the alarms
  rtcSetAlarm(rtcp, 0, NULL);
  rtcSetAlarm(rtcp, 1, NULL);

  //Get the current time
  rtcGetTime(&RTCD1, time);

  //get seconsd, add offset, make it a possible seconds value
  i = ((time->millisecond/1000) + offset) % 60;

  alarmspec.alrmr = (((uint32_t)(RTC_ByteToBcd2(i))) | \
		     ((uint32_t)(0x31) << 24)        | \
		     ((uint32_t)0x80808000)); 

  rtcSetAlarm(rtcp, 0, &alarmspec);
}

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
  char time_string[50];

  RTCDateTime time;
  struct tm ltime;
  (void)argv;
  (void)argc;
  rtcGetTime(&RTCD1, &time);
  rtcConvertDateTimeToStructTm(&time, &ltime, NULL);
  //  asctime_r(&ltime,time_string);
  //chprintf(chp,"%s\n\r",time_string);  
}

static void anabiosis(void) {
  chSysLock();
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  PWR->CR  |= (PWR_CR_PDDS | PWR_CR_LPDS | PWR_CR_CSBF | PWR_CR_CWUF);
  RTC->ISR &= ~(RTC_ISR_ALRBF | RTC_ISR_ALRAF | RTC_ISR_WUTF | RTC_ISR_TAMP1F |
                RTC_ISR_TSOVF | RTC_ISR_TSF);
  __WFI();
}

void cmd_enableWakeup(BaseSequentialStream *chp, int argc, char *argv[]) {
  /* RTCAlarm alarmspec; */

  /* rtcSetAlarm(&RTCD1,0,NULL); */
  /* rtcSetAlarm(&RTCD1,1,NULL); */
 
  static RTCWakeup wakeupspec;
  int wakeupPeriodSec = 1;
  
  wakeupPeriodSec = atoi(argv[0]);
  wakeupspec.wutr = ((uint32_t)4) << 16;  /* select 1 Hz clock source */
  wakeupspec.wutr |= (wakeupPeriodSec-1); /* Period will wakeupPeriodSec+1 seconds. */
  rtcSTM32SetPeriodicWakeup(&RTCD1, &wakeupspec);
  /* osalThreadSleepSeconds(3); */
  /* anabiosis(); */
}

void cmd_sleep(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: sleep\r\n");
    return;
  }
  chprintf(chp, "Going sleep...\r\n");
  chThdSleepMilliseconds(200);
  anabiosis();
}

/* clock.c ends here */
