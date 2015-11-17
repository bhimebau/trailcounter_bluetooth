/* adxl362.c --- 
 * 
 * Filename: adxl362.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Oct  1 15:28:36 2015
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
#include "adxl362.h"
#include "chprintf.h"
#include <atoh.h>
#include <stdlib.h>


/* SPI configuration, sets up PortA Bit 8 as the chip select for the adxl362 */
static SPIConfig adxl362_cfg = {
  NULL,
  GPIOB,
  6,
  SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0, 
  0
};

void cmd_reg(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  int i = 0x00;
  for(i; i<0x2F; i++){
    chprintf(chp, "Register 0x%x: 0x%x\t",i, adxl362_read_register(i));
    if (!(i%3))     chprintf(chp, "\n\r");
  }
}

void cmd_xyz(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;(void)argc; (void)argv;
  int i=0;
  //for(i; i<10; i++){
   while(1){
     //if( adxl362_read_register(0x0B)==0x41 ){
     // chprintf(chp, "INTERRUPT /n/r");
     //  }*/
    chprintf(chp, "X: %d, Y:%d, Z:%d ACT:%x \r\n",  adxl362_read_register(0x08), adxl362_read_register(0x09),  adxl362_read_register(0x0A), adxl362_read_register(0x0B) );
     }
}

uint8_t adxl362_read_register (uint8_t address) {
  uint8_t command = 0x0B;
  uint8_t dummy = 0x00;
  uint8_t receive_data;

  spiStart(&SPID1, &adxl362_cfg);       /* Setup transfer parameters. */
  spiSelect(&SPID1);                    /* Slave Select assertion. */
  spiSend(&SPID1, 1, &command);         /* Transmit Read Command */
  spiSend(&SPID1, 1, &address);         /* Read Command */
  spiReceive(&SPID1, 1, &receive_data); /* Read the data that is returned */
  spiUnselect(&SPID1);                  /* Slave Select de-assertion. */
  return (receive_data);
}

void adxl362_write_register (uint16_t address, uint8_t data) {
  uint8_t command = 0x0A;

  spiStart(&SPID1, &adxl362_cfg);      /* Setup transfer parameters. */
  spiSelect(&SPID1);                   /* Slave Select assertion.    */
  spiSend(&SPID1, 1, &command);        /* Write Command              */
  spiSend(&SPID1, 1, &address);        /* Address                    */ 
  spiSend(&SPID1, 1, &data);           /* Data                       */
  spiUnselect(&SPID1);                 /* Slave Select de-assertion. */
}

void adxl362_init (void) {
  //Read 0x0B status bit to reset interrupt

  // Pin Initializations
  palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(5));     /* SCK. */
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(5));     /* MISO.*/
  palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5));     /* MOSI.*/
  palSetPadMode(GPIOB, 6, PAL_MODE_OUTPUT_PUSHPULL);  /* adxl362 chip select */
  palSetPad(GPIOB, 6);                                /* Deassert the adxl362 chip select */


  //TODO:  play with activity/inactivity thresholds
  //TODO:  compare linked vs default modes vs LOOP(current)
  //TODO:  compare relative to absolute measurements
  //TODO:  play with all of this (for understanding)

  /*************
   *  Address:      0x1F
   *  Reset:        0x00
   *  Name:         SOFT_RESET
   *  Description:  Write 0x52 to reset the accelerometer
   *************/

  /*************
   *  Address:      0x20
   *  Reset:        0x00
   *  Name:         THRESH_ACT_L
   *  Description:  Holds eight least-significant-bits for thresholding activity detection
   *************/


  /*************
   *  Address:      0x21
   *  Reset:        0x00
   *  Name:         TRESH_ACT_H
   *  Description:  Holds three most-significant-bits for thresholding activity detection
   *************/


  /*************
   *  Address:      0x22
   *  Reset:        0x00
   *  Name:         TIME_ACT
   *  Description:  The amount of time that activity must persist to trigger activity detection
   *                0x00 or 0x01 both only require a single sample to be above threshold
   *************/


  /*************
   *  Address:      0x23
   *  Reset:        0x00
   *  Name:         THRESH_INACT_L
   *  Description:  Holds eight least-significant bits for thresholding inactivity detection
   *************/


  /*************
   *  Address:      0x24
   *  Reset:        0x00
   *  Name:         TRHESH_INACT_H
   *  Description:  Holds three most-significant bits for thresholding inactivity detection
   *************/


  /*************
   *  Address:      0x25
   *  Reset:        0x00
   *  Name:         TIME_INACT_L
   *  Description:  The eight least-significant bits corresponding to the amount of time that inactivity 
   *                must persist to trigger inactivity detection
   *************/


  /*************
   *  Address:      0x26
   *  Reset:        0x00
   *  Name:         TIME_INACT_H
   *  Description:  The eight most-significant bits corresponding to the amount of time that inactivity 
   *                must persist to trigger inactivity detection
   *************/


  /*************
   *  Address:      0x27
   *  Reset:        0x00
   *  Name:         ACT_INACT_CTL
   *  Description:  [7:6] Unused
   *                
   *************/


  /*************
   *  Address:      0x28
   *  Reset:        
   *  Name:         
   *  Description:  
   *************/


  /*************
   *  Address:      0x29
   *  Reset:        
   *  Name:         
   *  Description:  
   *************/


  /*************
   *  Address:      0x2A
   *  Reset:        
   *  Name:         
   *  Description:  
   *************/


  /*************
   *  Address:      0x2B
   *  Reset:        
   *  Name:         
   *  Description:  
   *************/


  /*************
   *  Address:      0x2C
   *  Reset:        
   *  Name:         
   *  Description:  
   *************/


  /*************
   *  Address:      0x2D
   *  Reset:        
   *  Name:         
   *  Description:  
   *************/


  /*************
   *  Address:      0x2E
   *  Reset:        
   *  Name:         
   *  Description:  
   *************/


  adxl362_write_register(0x20, 0x2C);  //thresh_act_l
  adxl362_write_register(0x21, 1);    //thresh_act_h
  adxl362_write_register(0x22, 0xA);    //thresh_act_h
  adxl362_write_register(0x23, 0x50);  //thresh_nact_l
  adxl362_write_register(0x24, 0);    //thresh_nact_h

  //amount of time to wait for inactivity
  //set to zero because we are trying not to use it (probably still does something)
  adxl362_write_register(0x25, 0);   //time_inact_l
  //adxl362_write_register(0x25, 0xC8);   //time_inact_l

  adxl362_write_register(0x26, 0);   //time_inact_l

  //Using loop mode, commented is attempt at default w/o inactivity
  adxl362_write_register(0x27, 0x3F); //act_inact_ctl
  //adxl362_write_register(0x27, 0x03); //act_inact_ctl

  adxl362_write_register(0x28, 0); //intmap2
  adxl362_write_register(0x29, 0x80); //intmap2
  adxl362_write_register(0x2A, 0x40); //intmap2
  adxl362_write_register(0x2B, 0); //intmap2
  adxl362_write_register(0x2C, 0x13); //intmap2
  adxl362_write_register(0x2D, 0x6); //power_ctl
  adxl362_write_register(0x2E, 0); //intmap2
  
}

void cmd_adxl362_read(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  int address = 0x00;
  if (argc == 1) {
    atoh(argv[0],&address);
    chprintf(chp,"0x%02x\n\r",adxl362_read_register(address));
  }
  else {
    chprintf(chp, "%d ERR\n\r", argc);
  }
}

void cmd_read_all(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  int address = 0x08;
  uint8_t output[3];
  int count=0;
  for(address; address<0x0B; address++){
    output[count] = adxl362_read_register(address);
    //returns all values of accel
    chprintf(chp, "%d: %d", count, output[count]);
    count++;
  }
}

void cmd_adxl362_write(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  int address = 0x00;
  int data = 0x00;
  if (argc == 2) {
    atoh(argv[0],&address);
    atoh(argv[1],&data);
    adxl362_write_register((uint16_t) address, (uint8_t) data);
  }
  else {
    chprintf(chp, "%d ERR\n\r", argc);
  }
}


/* adxl362.c ends here */
