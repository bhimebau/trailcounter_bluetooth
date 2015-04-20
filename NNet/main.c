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
void dc_sensitivity_test(void) {
  float dc_input;
  int i;
  for (dc_input=-5.0;dc_input<5.0;dc_input+=0.1) {
    for (i=0;i<100;i++) {
      EvaluateNet(dc_input);
    }    
    if ((outputs[0]>0.5) || ((dc_input>-0.1) && (dc_input<0.1))){
      printf("DC Value %f, %f\n", dc_input, outputs[0]);
    }
  }
}


int main(){
  int i;
  dc_sensitivity_test();

  // Clear shift register in inputs
  for (i=0;i<32;i++) {
    EvaluateNet(0.0);
  }
  for (i=0;i<32;i++) {
    scaled_input = nntest_data_sampled[i] * ARB_SCALE_FACTOR + ARB_OFFSET;
    EvaluateNet(scaled_input);
    printf("%d, %f\n", i, outputs[0]);
  }
  return 0; 
}
/* main.c ends here */
