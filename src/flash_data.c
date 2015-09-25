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
#define MAX_DAYS 1000

volatile uint32_t epoch_data[MAX_DAYS] __attribute__((section (".rodata"))) = { [0 ... MAX_DAYS-1] = 0xFFFFFFFF};
volatile uint16_t hourly_data[24*MAX_DAYS] __attribute__((section (".rodata"))) = { [0 ... (24*MAX_DAYS)-1] = 0xFFFF};

void writeEpochData(


/* flash_data.c ends here */
