/* caann.h --- 
 * 
 * Filename: caann.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Sun Sep 20 12:13:11 2015
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

/* Copyright (c) 2014-2015 Analog Computing Solutions  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
void caann_init (void);
void caann_write_register (uint8_t, uint8_t);
uint8_t caann_read_register (uint8_t);
void cmd_caann_read(BaseSequentialStream *, int, char **);

/* caann.h ends here */
