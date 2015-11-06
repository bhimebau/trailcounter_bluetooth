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
  // Pin Initializations
  palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(5));     /* SCK. */
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(5));     /* MISO.*/
  palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5));     /* MOSI.*/
  palSetPadMode(GPIOB, 6, PAL_MODE_OUTPUT_PUSHPULL);  /* adxl362 chip select */
  palSetPad(GPIOB, 6);                                /* Deassert the adxl362 chip select */
  int reg = 0x20;
  for(reg;reg<0x27;reg++){
    if(reg == 0x22){
      adxl362_write_register(reg, 0x02);              /*TIME_ACT register >1 to avoid false pos. */
    }
    else{
      adxl362_write_register(reg, 0x01);              /*Set Activity/Inactivity thresholds/timers*/
    }
  }
 

  adxl362_write_register(0x27, 0x00);           /* reg = 0x27 Control Reg: Default.*/
  adxl362_write_register(0x28, 0x03);           /* FIFO - Triggered Mode */
  adxl362_write_register(0x29, 0x60);           /* Number of FIFO samples to keep */
  adxl362_write_register(0x2A, 0x40);           /*map Awake status to INT1 Pin */
  adxl362_write_register(0x2B, 0x20);           /*map inactivity status to INT2 pin. */
  adxl362_write_register(0x2C, 0x10);           /* +- 2g, thinner bandwidth, 12.5 hz(low) */
  adxl362_write_register(0x2D, 0x02);           /* measurement mode */

  
  //adxl362_write_register(0x27, 0x00);           /* reg = 0x27 Control Reg: Default.*/
  //adxl362_write_register(0x28, 0x03);           /* FIFO - Triggered Mode */
  //adxl362_write_register(0x29, 0x60);           /* Number of FIFO samples to keep */
  //adxl362_write_register(0x2A, 0x40);           /*map Awake status to INT1 Pin */
  //adxl362_write_register(0x2B, 0x20);           /*map inactivity status to INT2 pin. */
  //adxl362_write_register(0x2C, 0x10);           /* +- 2g, thinner bandwidth, 12.5 hz(low) */
  //adxl362_write_register(0x2D, 0x02);           /* measurement mode */


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
