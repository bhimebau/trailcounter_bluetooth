/* caann.c --- 
 * 
 * Filename: caann.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Sun Sep 20 10:57:35 2015
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
#include "caann.h"
#include "atoh.h"
#include "chprintf.h"

/* SPI configuration, sets up PortA Bit 8 as the chip select for the caann */
static SPIConfig caann_cfg = {
  NULL,
  GPIOA,
  8,
  SPI_CR1_BR_2 | SPI_CR1_BR_1,
  0
};

uint8_t caann_read_register (uint8_t address) {
  uint8_t receive_data;

  address = address | 0x80;            /* Set the read bit (bit 7)         */
  spiStart(&SPID1, &caann_cfg);         /* Setup transfer parameters. */
  spiSelect(&SPID1);                   /* Slave Select assertion.          */
  spiSend(&SPID1, 1, &address);        /* Send the address byte */
  spiReceive(&SPID1, 1,&receive_data); 
  spiUnselect(&SPID1);                 /* Slave Select de-assertion.       */
  return (receive_data);
}

void caann_write_register (uint8_t address, uint8_t data) {
  address = address & (~0x80);         /* Clear the write bit (bit 7)      */
  spiStart(&SPID1, &caann_cfg);         /* Setup transfer parameters. */
  spiSelect(&SPID1);                   /* Slave Select assertion.          */
  spiSend(&SPID1, 1, &address);        /* Send the address byte            */
  spiSend(&SPID1, 1, &data); 
  spiUnselect(&SPID1);                 /* Slave Select de-assertion.       */
}

void caann_init (void) {
  // Pin Initializations
  palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(5));     /* SCK. */
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(5));     /* MISO.*/
  palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5));     /* MOSI.*/
  palSetPadMode(GPIOA, 8, PAL_MODE_OUTPUT_PUSHPULL);  /* caann chip select */
  palSetPad(GPIOA, 8);                                /* Deassert the caann chip select */
}

void cmd_caann_read(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i = 0;
  (void)argv;

  int address = 0x00;
  if (argc == 1) {
    atoh(argv[0],&address);
    chprintf(chp,"%x\n\r",caann_read_register(address));
  }
  else {
    chprintf(chp, "%ERR\n\r", argv[i]);
  }
}


/* caann.c ends here */
