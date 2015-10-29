/* wakesfromstopwithalarm.c
 * 
 * Filename: wakesfromstopwithalarm.c
 * Description: current working example for waking from low power mode with alarm
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Sun Sep 20 09:32:19 2015
 * Last-Updated: 
 *           By: Billie
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
volatile int alarm_called;

static void extcb(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;

  chSysLockFromISR();

  stm32_clock_init();
  alarm_called++;

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
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA , extcb},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

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


/*
 * Application entry point.
 */
int main(void) {
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
  extStart(&EXTD1, &trailExtcfg);
  extChannelEnable(&EXTD1, EXT_MODE_GPIOA);

  alarm_called = 0;

  //prev is tracking alarm_called change
  //i is alarm timer in seconds
  //j is temporary variable to store the time it's set to wake up
  //k is temp variable to store new time to wake up, gotten from populate
  //alarmspec and alarmspec1 are structures to store/load the current time
  int prev = 1;
  int i = 5;
  int j;
  uint8_t k;
  RTCAlarm alarmspec1, alarmspec;

  while (TRUE){
    if (alarm_called != prev) { 
      prev = alarm_called;

      rtcGetTime(&RTCD1, &time);
      rtcConvertDateTimeToStructTm(&time,&ltime, NULL);

      //alarmpopulate from ~/bridge-troll/beta/trail_counter_rtc_time.c on Billie local machine
      k = (ltime.tm_sec + i) % 60;
      alarmspec.alrmr = (((uint32_t)(RTC_ByteToBcd2(k))) | \
			 ((uint32_t)(0x31) << 24)        | \
			 ((uint32_t)0x80808000)); 

      rtcSetAlarm(&RTCD1, 0, NULL);
      rtcSetAlarm(&RTCD1, 1, NULL);

      rtcSetAlarm(&RTCD1, 0, &alarmspec);

      chprintf((BaseSequentialStream*)&SD2,"AWAKE!\n\rCurrent count:  %d\r\n",alarm_called);
      chprintf((BaseSequentialStream*)&SD2,"Current time%s\r",asctime(&ltime));

      rtcGetAlarm(&RTCD1, 0, &alarmspec1);
      j = (alarmspec1.alrmr & 0b1111) + ((alarmspec1.alrmr >> 4) & 0b111) * 10;
      chprintf((BaseSequentialStream*)&SD2, "Next alarm is set to %U%s\n\r\n\r", j);
    }

    chThdSleepMilliseconds(1000);
    PWR_EnterSTOPMode( ((uint32_t)0x00000001), PWR_STOPEntry_WFI);
  }
  return 0;

}


/* main.c ends here */
