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
#include "led.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>


/* Implementation of External Interrupts */

volatile int alarm_called;
volatile uint16_t people_count;

/** Callback for Accelerometer Wakeup **/
static void extcb(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;

  chSysLockFromISR();
  people_count++;
  //stm32_clock_init();
  chSysUnlockFromISR();
}


/** Callback for ALARM wakeup **/
static void extcb1(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;

  chSysLockFromISR();
  //power consumption and additional time need to be tested
  //also make sure all functionality remains the same
  //stm32_clock_init();
  alarm_called = 1;
  chSysUnlockFromISR();
}

/* Callback for button press */
static void buttoncb(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;
  chSysLockFromISR();
  led_on();
  chSysUnlockFromISR();
}

// STM32F303 EXT Interrupt Interface 
EXTConfig trailExtcfg = {
  {
    // (0)  Bit 0 
    {EXT_CH_MODE_DISABLED, NULL},
    // (1) Bit 1 Enable interrupt on PA1 from the accelerometer
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, extcb},
    // (2) Bit 2
    {EXT_CH_MODE_DISABLED, NULL},
    // (3) Bit 3
    {EXT_CH_MODE_DISABLED, NULL},
    // (4) Bit 4
    {EXT_CH_MODE_DISABLED, NULL},
    // (5) Bit 5
    {EXT_CH_MODE_DISABLED, NULL},
    // (6) Bit 6
    {EXT_CH_MODE_DISABLED, NULL},
    // (7) Bit 7 Enable falling edge of button. 
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, buttoncb},
    // (8) Bit 8
    {EXT_CH_MODE_DISABLED, NULL},
    // (9) Bit 9
    {EXT_CH_MODE_DISABLED, NULL},
    // (10) Bit 10
    {EXT_CH_MODE_DISABLED, NULL},
    // (11) Bit 11
    {EXT_CH_MODE_DISABLED, NULL},
    // (12) Bit 12
    {EXT_CH_MODE_DISABLED, NULL},
    // (13) Bit 13
    {EXT_CH_MODE_DISABLED, NULL},
    // (14) Bit 14
    {EXT_CH_MODE_DISABLED, NULL},
    // (15) Bit 15
    {EXT_CH_MODE_DISABLED, NULL},
    // (16) PVD Output
    {EXT_CH_MODE_DISABLED, NULL},
    // (17) RTC Alarm: Enable interrupt from the RTC alarm
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA , extcb1},
    // (18) USB FS Wakeup
    {EXT_CH_MODE_DISABLED, NULL},
    // (19) RTC Tamper and Timestamps
    {EXT_CH_MODE_DISABLED, NULL},
    // (20) RTC Wakeup 
    {EXT_CH_MODE_DISABLED, NULL},
    // (21) Comparator 1 output
    {EXT_CH_MODE_DISABLED, NULL},
    // (22) Comparator 2 output
    {EXT_CH_MODE_DISABLED, NULL},
    // (23) 12C1 Wakeup
    {EXT_CH_MODE_DISABLED, NULL},
    // (24) I2C2 Wakeup 
    {EXT_CH_MODE_DISABLED, NULL},
    // (25) USART1 Wakeup
    {EXT_CH_MODE_DISABLED, NULL},
    // (26) USART2 Wakeup
    {EXT_CH_MODE_DISABLED, NULL},
    // (27) Reserved
    {EXT_CH_MODE_DISABLED, NULL},
    // (28) USART3 Wakeup
    {EXT_CH_MODE_DISABLED, NULL},
    // (29) Comparator 3 output
    {EXT_CH_MODE_DISABLED, NULL},
    // (30) Comparator 4 output 
    {EXT_CH_MODE_DISABLED, NULL},
    // (31) Comparator 5 output 
    {EXT_CH_MODE_DISABLED, NULL},
    // (32) Comparator 6 output 
    {EXT_CH_MODE_DISABLED, NULL},
    // (33) Comparator 7 output 
    {EXT_CH_MODE_DISABLED, NULL},
    // (34) UART 4 wakeup
    {EXT_CH_MODE_DISABLED, NULL},
    // (35) UART 5 wakeup 
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
  (void) chp;

  RTCDateTime time;
  struct tm ltime;
  (void)argv;
  (void)argc;
  rtcGetTime(&RTCD1, &time);
  rtcConvertDateTimeToStructTm(&time, &ltime, NULL);
  asctime_r(&ltime,time_string);
  chprintf(chp,"%s\n\r",time_string);  
}

/* static void anabiosis(void) { */
/*   chSysLock(); */
/*   SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; */
/*   PWR->CR  |= (PWR_CR_PDDS | PWR_CR_LPDS | PWR_CR_CSBF | PWR_CR_CWUF); */
/*   RTC->ISR &= ~(RTC_ISR_ALRBF | RTC_ISR_ALRAF | RTC_ISR_WUTF | RTC_ISR_TAMP1F | */
/*                 RTC_ISR_TSOVF | RTC_ISR_TSF); */
/*   __WFI(); */
/* } */

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
