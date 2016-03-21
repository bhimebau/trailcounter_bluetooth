/*
  Copyright (c) 2004-2016 The Trustees of Indiana University and Indiana
  University Research and Technology Corporation.

  All rights reserved.

  Additional copyrights may follow.
*/

uint8_t adxl362_read_register (uint8_t address);
void adxl362_write_register (uint16_t address, uint8_t data);
void adxl362_init (void);
void cmd_adxl362_read(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_adxl362_write(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_xyz(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_reg(BaseSequentialStream *chp, int argc, char *argv[]);
