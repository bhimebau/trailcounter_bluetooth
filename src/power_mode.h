/* power_mode.h --- 
 * 
 * Filename: power_mode.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Mon Feb 15 10:09:39 2016
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

void power_enter_sleep_mode(void);
void mcu_sleep(void);
void mcu_wake(void);
void DeepSleepDisableI(bool);
void SleepNextIdleI(bool clock_init);

extern int ClockInited;
extern int SleepThreadCounter;

/* power_mode.h ends here */
