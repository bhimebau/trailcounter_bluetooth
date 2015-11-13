/* flash_data.c --- 
 * 
 * Filename: flash_data.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Fri Sep 25 17:00:38 2015
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


volatile uint32_t epoch_data[MAX_DAYS] __attribute__((section (".rodata"))) = { [0 ... MAX_DAYS-1] = 0xFFFFFFFF};
volatile uint16_t hourly_data[24*MAX_DAYS] __attribute__((section (".rodata"))) = { [0 ... (24*MAX_DAYS)-1] = 0xFFFF};

void writeEpochDataWord(int epoch_array_index, int data) {
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  FLASH_ProgramWord((uint32_t)(&epoch_data[epoch_array_index]),data);
  FLASH_Lock();
}

void writeHourlyData(int hourly_array_index, uint16_t data) {
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  FLASH_ProgramHalfWord((uint32_t)(&hourly_data[hourly_array_index]),data);
  FLASH_Lock();
}

int getFirstFreeEpoch(void) {
  int i;
  for (i=0;i<MAX_DAYS;i++) {
    if (epoch_data[i] == 0xFFFFFFFF) {
      break;
    }
  } 
  return i;
}

int getFirstFreeHourly(void) {
  int i;
  for (i=getFirstFreeEpoch()*24;i<MAX_DAYS*24;i++) {
    if (hourly_data[i] == 0xFFFF) {
      break;
    }
  } 
  return i;
}

int printEpochData(void) {
  int i;
  for (i=0;i<MAX_DAYS;i++) {
    if (epoch_data[i] == 0xFFFFFFFF) {
      break;
    }
    else {
      chprintf((BaseSequentialStream*)&SD2,"index=%d,data=%d\n\r",i,epoch_data[i]);
    }
  } 
  return i;
}

int printHourlyData(void) {
  int i;
  for (i=0;i<MAX_DAYS*24;i++) {
    if (hourly_data[i] == 0xFFFF) {
      break;
    }
    else {
      chprintf((BaseSequentialStream*)&SD2,"index=%d,data=%d\n\r",i,hourly_data[i]);
    }
  } 
  return i;
}

int printCoalesceData(void) {
  int i;
  for (i=0;i<MAX_DAYS*24;i++) {
    if (hourly_data[i] == 0xFFFF) {
      break;
    }
    if (i % 24 == 0) {
      chprintf((BaseSequentialStream*)&SD2,"\n\r---Day---\n\r");
      chprintf((BaseSequentialStream*)&SD2,"Day #: %d, Data: %x\n\r",i%24,epoch_data[i%24]);
      chprintf((BaseSequentialStream*)&SD2,"---Hour---\n\r");
    }
    chprintf((BaseSequentialStream*)&SD2,"Hour #: %d, Data: %x\n\r",i,hourly_data[i]);
  }
  return i;
}






/* flash_data.c ends here */
