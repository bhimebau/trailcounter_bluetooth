/*
  Copyright (c) 2004-2016 The Trustees of Indiana University and Indiana
  University Research and Technology Corporation.

  All rights reserved.

  Additional copyrights may follow.
*/

#define MAX_DAYS 1000

extern volatile uint32_t epoch_data[];
extern volatile uint16_t hourly_data[];

void writeEpochDataWord(int, int);
void writeHourlyData(int hourly_array_index, uint16_t data);
int getFirstFreeEpoch(void);
int getFirstFreeHourly(void);
int printEpochData(void);
int printHourlyData(void);
