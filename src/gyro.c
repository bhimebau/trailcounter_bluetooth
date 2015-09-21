/* gyro.c --- 
 * 
 * Filename: gyro.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Sun Sep 20 09:46:37 2015
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
#include "ch.h"
#include "hal.h"
#include "gyro.h"
#include "atoh.h"
#include "chprintf.h"

/* SPI configuration, sets up PortE Bit 3 as the chip select for the gyro */
static SPIConfig gyro_cfg = {
  NULL,
  GPIOE,
  3,
  SPI_CR1_BR_2 | SPI_CR1_BR_1,
  0
};

uint8_t gyro_read_register (uint8_t address) {
  uint8_t receive_data;

  address = address | 0x80;            /* Set the read bit (bit 7)         */
  spiStart(&SPID1, &gyro_cfg);         /* Setup transfer parameters. */
  spiSelect(&SPID1);                   /* Slave Select assertion.          */
  spiSend(&SPID1, 1, &address);        /* Send the address byte */
  spiReceive(&SPID1, 1,&receive_data); 
  spiUnselect(&SPID1);                 /* Slave Select de-assertion.       */
  return (receive_data);
}

void gyro_write_register (uint8_t address, uint8_t data) {
  address = address & (~0x80);         /* Clear the write bit (bit 7)      */
  spiStart(&SPID1, &gyro_cfg);         /* Setup transfer parameters. */
  spiSelect(&SPID1);                   /* Slave Select assertion.          */
  spiSend(&SPID1, 1, &address);        /* Send the address byte            */
  spiSend(&SPID1, 1, &data); 
  spiUnselect(&SPID1);                 /* Slave Select de-assertion.       */
}

void gyro_init (void) {
  // Pin Initializations
  palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(5));     /* SCK. */
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(5));     /* MISO.*/
  palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5));     /* MOSI.*/
  palSetPadMode(GPIOE, 3, PAL_MODE_OUTPUT_PUSHPULL);  /* gyro chip select */
  palSetPad(GPIOE, 3);                                /* Deassert the gyro chip select */
}

void cmd_gyro_read(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i = 0;
  (void)argv;

  int address = 0x00;
  if (argc == 1) {
    atoh(argv[0],&address);
    chprintf(chp,"%2x\n\r",gyro_read_register(address));
  }
  else {
    chprintf(chp, "%ERR\n\r", argv[i]);
  }
}


/* gyro.c ends here */
