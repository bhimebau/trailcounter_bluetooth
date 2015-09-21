/* gyro.h --- 
 * 
 * Filename: gyro.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Sun Sep 20 11:07:16 2015
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
void gyro_init (void);
void gyro_write_register (uint8_t, uint8_t);
uint8_t gyro_read_register (uint8_t);
void cmd_gyro_read(BaseSequentialStream *, int, char **);

/* gyro.h ends here */
