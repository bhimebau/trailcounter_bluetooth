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
  adxl362_write_register(0x20, 0x5A);  //Started at 2C, then 96(12C/2).  4B was too sensitive. 73 was too high. 

  /*************
   *  Address:      0x21
   *  Reset:        0x00
   *  Name:         THRESH_ACT_H
   *  Description:  Holds three most-significant-bits for thresholding activity detection
   *************/

  adxl362_write_register(0x21, 0);   //was 1. Then 0. 


  /*************
   *  Address:      0x22
   *  Reset:        0x00
   *  Name:         TIME_ACT
   *  Description:  The amount of time that activity must persist to trigger activity detection
   *                0x00 or 0x01 both only require a single sample to be above threshold
   *************/
  adxl362_write_register(0x22, 0xA);  //10 Samples  

  /*************
   *  Address:      0x23
   *  Reset:        0x00
   *  Name:         THRESH_INACT_L
   *  Description:  Holds eight least-significant bits for thresholding inactivity detection
   *************/
  adxl362_write_register(0x23, 0x50);  


  /*************
   *  Address:      0x24
   *  Reset:        0x00
   *  Name:         TRHESH_INACT_H
   *  Description:  Holds three most-significant bits for thresholding inactivity detection
   *************/

  adxl362_write_register(0x24, 0);   

  /*************
   *  Address:      0x25
   *  Reset:        0x00
   *  Name:         TIME_INACT_L
   *  Description:  The eight least-significant bits corresponding to the amount of time 
   *                that inactivity must persist to trigger inactivity detection
   *************/

  //set to zero because we are trying not to use it (probably still does something)
  adxl362_write_register(0x25, 0);  
  //adxl362_write_register(0x25, 0xC8);   

  /*************
   *  Address:      0x26
   *  Reset:        0x00
   *  Name:         TIME_INACT_H
   *  Description:  The eight most-significant bits corresponding to the amount of time 
   *                that inactivity must persist to trigger inactivity detection
   *************/

  adxl362_write_register(0x26, 0);  

  /*************
   *  Address:      0x27
   *  Reset:        0x00
   *  Name:         ACT_INACT_CTL
   *  Description:  [7:6] Unused. 
   *                [5:4] LINK/LOOP:
   *                        0X - Default Mode (Activity and Inactivity are both enabled
   *                             interrupts must be acknowledges by host Processor by 
   *                             reading STATUS register. Autosleep disabled.)
   *                      
   *                        01 - Linked Mode (Act and Inact linked sequesntially such that 
   *                             only one is enabled at a given time. Must be ack'd by Host
   *                             processor. Bit 0(ACT_EN) and 1(INACT_EN) must be 1.
   *
   *                        11 - Loop Mode (Act and Inact detection are linked sequentially
   *                             so only one is enabled at a time. Interr's don't need acknowledgement
   *                             Bit 0(ACT_EN) and 1(INACT_EN) must be 1.
   *
   *                [3]   INACT_REF:
   *                        1 = inactivity in referenced mode. (movement is tracked from current
   *                        position, so it can be any axis)
   *                        0 = inactivity in absolute mode. (Movement is tracked on a specific 
   *                        position.)
   *
   *                [2]   INACT_EN: 
   *                        1 = enables inactivity (underthreshold) functionality.
   *          
   *                [1]   ACT_REF:
   *                        1 = activity in referenced mode. (movement is tracked from current
   *                        position, so it can be any axis)
   *                        0 = activity in absolute mode. (Movement is tracked on a specific 
   *                        position.)
   *
   *                [0]   ACT_EN: 
   *                        1 = enables activity (overthreshold) functionality.
   *
   *                
   *************/

  //Using loop mode, commented is attempt at default w/o inactivity
  adxl362_write_register(0x27, 0x3F); 
  //adxl362_write_register(0x27, 0x03); 


  /*************
   *  Address:      0x28
   *  Reset:        0x00
   *  Name:         FIF0_CONTROL
   *  Description:  
   *               [7:4] -  UNUSED
   *
   *               [3] - AH:     
   *                       Above Half, This bit is MASB of the FIFO_SAMPLES register allowing
   *                       FIFO samples a range of 0 to 511.
   *
   *               [2] - FIFO_TEMP:
   *                       1 = temp data is stored in the FIFO together with axis data.
   *
   *               [1:0] - FIFO_MODE:
   *                       00 - FIFO Disabled.
   *                       01 - Oldest saved mode.
   *                       10 - Stream mode.
   *                       11 - Triggered mode.
   *
   *************/
  adxl362_write_register(0x28, 0);

  /*************
   *  Address:      0x29
   *  Reset:        0x80
   *  Name:         FIFO_SAMPLES
   *  Description:  Specifies number of samples to store in FIFO. The MSB of this value is  
   *                the AH bit in the FIFO_CONTROL[3]. 0x80 avoids triggering FIFO watermark.
   *************/

  adxl362_write_register(0x29, 0x80); 

  /*************
   *  Address:      0x2A
   *  Reset:        0x00
   *  Name:         INTMAP1
   *  Description:  Settings for Interrupt (INT) 1. All enabled settings are OR'd together.
   *
   *                [7] - INT_LOW:
   *                       1=INT1 pin active low.
   *                
   *                [6] - AWAKE:
   *                       1=maps awake status to INT1 pin.
   *
   *                [5] - INACT: 
   *                       1=maps inactivity status to INT1 pin.
   *
   *                [4] - ACT:
   *                       1 = maps activity status to INT1 pin.
   *
   *                [3] - FIFO_OVERRUN:
   *                       1 = maps FIFO overrun status to INT1 pin.
   *
   *                [2] - FIFO_WATERMARK:
   *                       1 = maps FIFO watermark status to INT1_pin.
   *
   *                [1] - FIFO_READY:
   *                       1 = maps FIFO ready status to INT1 pin.
   *
   *                [0] - DATA_READY:
   *                       1 = maps data ready status to INT1 pin.
   *************/
 
  adxl362_write_register(0x2A, 0x40); //maps ONLY awake status.

  /*************
   *  Address:      0x2B
   *  Reset:        0x00
   *  Name:         INTMAP2
   *  Description:  Settings for Interrupt (INT)2. All enabled settings are OR'd together.
   *
   *                [7] - INT_LOW:
   *                       1=INT2 pin active low.
   *                
   *                [6] - AWAKE:
   *                       1=maps awake status to INT2 pin.
   *
   *                [5] - INACT: 
   *                       1=maps inactivity status to INT2 pin.
   *
   *                [4] - ACT:
   *                       1 = maps activity status to INT2 pin.
   *
   *                [3] - FIFO_OVERRUN:
   *                       1 = maps FIFO overrun status to INT2 pin.
   *
   *                [2] - FIFO_WATERMARK:
   *                       1 = maps FIFO watermark status to INT2_pin.
   *
   *                [1] - FIFO_READY:
   *                       1 = maps FIFO ready status to INT2 pin.
   *
   *                [0] - DATA_READY:
   *                       1 = maps data ready status to INT2 pin.
   *************/
  adxl362_write_register(0x2B, 0); //INT2 not used.

  /*************
   *  Address:      0x2C
   *  Reset:        0x13
   *  Name:         FILTER_CTL
   *  Description:  
   *                [7:6] - RANGE:
   *                      00 = +- 2g(reset default)
   *                      01 = +- 4g
   *                      1X = +- 8g
   *
   *                  [5] - RES: reserved
   *                        
   *                  [4] - HALF_BW:
   *                      1 = bandwidth of antialiasing filters is 1/4 of the ODR for more 
   *                       conservative filtering.
   *                      0 = bandwidth is set to 1/2 the ODR for wider bandwidth.
   *                  
   *                  [3] - EXT_SAMPLE:
   *                      1 = the INT2 pin is used for external conversion timing control. 
   *
   *                [2:0] - ODR (Output Data Rate):
   *                    000 = 12.5 Hz      
   *                    001 = 25 Hz      
   *                    010 = 50 Hz      
   *                    011 = 100 Hz (reset default)      
   *                    100 = 200 Hz      
   *              101...111 = 400 Hz      
   *
   *************/
  adxl362_write_register(0x2C, 0x13); // 1/4 ODR, 100Hz

  /*************
   *  Address:      0x2D
   *  Reset:        0x00
   *  Name:         POWER_CTL
   *  Description:  Power settings
   *
   *                 [7] - Reserved.
   *
   *                 [6] - EXT_CLK:
   *                         External clock. 1 = accel runs off ext clock provided on INT1 pin.
   *
   *               [5:4] - LOW_NOISE: (Selects power vs. Noise Tradeoff)
   *                     00 = Normal operation (reset default)
   *                     01 = Low Noise mode
   *                     10 = Ultralow noise mode
   *                     11 = reserved.
   *
   *                 [3] - WAKEUP:
   *                      1 = the part operates in wake-up Mode.    
   *
   *                 [2] - AUTOSLEEP:
   *                      1 = autosleep is enabled, and the device enters wake-up mode automatically
   *                          upon detection of inactivity.
   *
   *               [1:0] - MEASURE:
   *                     00 = Stanby
   *                     01 = Reserved
   *                     10 = Measurement mode
   *                     11 = Reserved
   *
   *************/
  adxl362_write_register(0x2D, 0x6); //Wake-up, Autosleep.

  /*************
   *  Address:      0x2E
   *  Reset:        0x00
   *  Name:         SELF_TEST
   *  Description:  [7:1] UNUSED
   *                  [0] SelfTest, 1 = a self test force is applied to axes.
   *************/

  adxl362_write_register(0x2E, 0); 
  
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
