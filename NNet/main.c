/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Mon Feb  2 15:09:52 2015
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

#include "NNet.h"
#include <arb.h>
#include <stdio.h>

float scaled_input;

int main(){
  int i;
 
  for (i=0;i<32;i++) {
    scaled_input = nntest_data_sampled[i] * scale_factor + offset;
    EvaluateNet(scaled_input);
    printf("%d, %f\n", i, outputs[0]);
  }

  return 0; 
}
/* main.c ends here */
