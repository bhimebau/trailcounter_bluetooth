/* power_mode.c --- 
 * 
 * Filename: power_mode.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Mon Feb 15 10:07:26 2016
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

int ClockInited = false;
int SleepThreadCounter = 1;

void power_enter_sleep_mode(void) {
  SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);		
  __WFI();								
}

// Called at IDLE Entry
void mcu_sleep(void)  {
  /* Select the regulator state in STOP mode ---------------------------------*/
  uint32_t tmpreg = PWR->CR;
  
  /* Clear PDDS and LPDS bits */
  tmpreg &= (uint32_t)~(PWR_CR_PDDS | PWR_CR_LPDS);

  /* Set LPDS bit according to Regulator value */
  tmpreg |= PWR_CR_LPDS | PWR_CR_CSBF | PWR_CR_CWUF;

  /* Store the new value */
  PWR->CR = tmpreg;
  
  // save register
  tmpreg = SCB->SCR;
  
  // select regular sleep mode
  SCB->SCR = tmpreg & ~SCB_SCR_SLEEPDEEP_Msk;
  
  // atomically compare flag, ...
  __disable_irq();
  
  // clear extra pending bit (this works in IRQ disabled context)
  __SEV();
  __WFE();
  if (SleepThreadCounter == 0) {
    // Deep sleep mode
    // enable deep sleep
    SCB->SCR = tmpreg | SCB_SCR_SLEEPDEEP_Msk;
    
    // deep sleep disables the clocks, need reinitializition
    ClockInited = false;
    __enable_irq();
    
    //
    // Unwanted Interrupt can be only here, but it set SEV (see function DeepSleepDisableI()) ,
    // if no need to sleep and will skip WFE insruction
    
    // wait an event - this not work in IRQ disabled context!
    __WFE();
  } 
  else {
    __enable_irq();
    // Regular sleep mode
    // wait an interrupt
    __WFI();
    led_on();
  }  
}

// Called at Idle Exit 
void mcu_wake(void) {
  if (!ClockInited){
    stm32_clock_init();
  }
}

void DeepSleepDisableI(bool clock_init) {
  // set extra pending bit to disable unwanted sleep!
  __SEV();
  ++SleepThreadCounter;
}  

void SleepNextIdleI(bool clock_init) {
  // set extra pending bit to disable unwanted sleep!
  //   __SEV();
  SleepThreadCounter = 0;
}  

/* power_mode.c ends here */
