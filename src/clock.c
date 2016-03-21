/*
  Copyright (c) 2004-2016 The Trustees of Indiana University and Indiana
  University Research and Technology Corporation.

  All rights reserved.

  Additional copyrights may follow.
*/

/*
  Authors: bhimebau

  Driver file for realtime clock.
*/

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


// Begin Implementation of External Interrupts

volatile int alarm_called;
volatile uint16_t people_count;

// Callback for Accelerometer Wakeup
static void extcb(EXTDriver *extp, expchannel_t channel) {
  (void) extp;
  (void) channel;

  chSysLockFromISR();
  people_count++;
  //stm32_clock_init();
  chSysUnlockFromISR();
}


// Callback for ALARM wakeup
static void extcb1(EXTDriver *extp, expchannel_t channel) {
  (void) extp;
  (void) channel;

  chSysLockFromISR();

  // FIXME: This comment is really unclear; what functionality?
  /*
    TODO(power): Test power consumption and additional time.
    Make sure all functionality remains the same.
    stm32_clock_init();
  */
  alarm_called = 1;
  chSysUnlockFromISR();
}

EXTConfig trailExtcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    // Enable interrupt on PA1 from the accelerometer
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

// End Implementation of External Interrupts

// FIXME: Why not have this as a macro?
static uint8_t RTC_ByteToBcd2(uint8_t value) {
  return  ((uint8_t)((value / 10) << 4) | value);
}

void trailRtcInitAlarmSystem(void) {
  extStart(&EXTD1, &trailExtcfg);
  extChannelEnable(&EXTD1, EXT_MODE_GPIOA);
}

void trailRtcSetAlarm(RTCDriver *rtcp, uint8_t offset, RTCDateTime *time) {
  uint8_t i;
  RTCAlarm alarmspec;

  // Reset the alarms.
  rtcSetAlarm(rtcp, 0, NULL);
  rtcSetAlarm(rtcp, 1, NULL);

  // Get the current time.
  rtcGetTime(&RTCD1, time);

  // Get seconds, add offset, make it a possible seconds value.
  i = ((time->millisecond / 1000) + offset) % 60;

  alarmspec.alrmr = (((uint32_t)(RTC_ByteToBcd2(i))) | \
		((uint32_t)(0x31) << 24) | \
		((uint32_t)0x80808000));

  rtcSetAlarm(rtcp, 0, &alarmspec);
}

// Sets the rtc to a given UTC time.
void cmd_rtcSet(BaseSequentialStream *chp, int argc, char *argv[]) {
  RTCDateTime time;
  struct tm * ltime;
  time_t epoch;

  if (argc != 1) {
    chprintf(chp, "Incorrect number of arguments: %d\n\r", argc);
  }

  epoch = (time_t) atoi(argv[0]);
  epoch -= 4 * 3600; // Convert from UTC time to EDT time.
  ltime = localtime(&epoch);
  rtcConvertStructTmToDateTime(ltime, 0, &time);
  rtcSetTime(&RTCD1, &time);
}

// Prints the current rtc time.
void cmd_rtcRead(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void) argc;
  (void) argv;

  int32_t i;
  char time_string[50];

  RTCDateTime time;
  struct tm ltime;
  rtcGetTime(&RTCD1, &time);
  rtcConvertDateTimeToStructTm(&time, &ltime, NULL);
  asctime_r(&ltime, time_string);

  chprintf(chp, "%s\n\r", time_string);
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

  (void) argc;
  (void) chp;
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
  (void) argv;

  if (argc > 0) {
    chprintf(chp, "Usage: sleep\r\n");
    return;
  }
  chprintf(chp, "Going to sleep...\r\n");
  chThdSleepMilliseconds(200);
  anabiosis();
}
